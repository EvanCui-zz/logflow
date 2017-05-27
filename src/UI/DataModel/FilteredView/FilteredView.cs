namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Reflection;
    using System.Threading;
    using System.Threading.Tasks;

    public class FilteredView<T> : IFilteredView<T>, IDisposable where T : DataItemBase
    {
        #region Find, Count, Filter, Tag, Indent features.

        public int GetLogicalIndexOfItem(int itemId)
        {
            if (this.ItemIndices != null)
            {
                return this.ItemIndices.BinarySearch(itemId);
            }
            else
            {
                return itemId;
            }
        }

        public void UnIndentActivity(int actIdIndex)
        {
            this.indentedActIdIndices.Remove(actIdIndex);
        }

        public void IndentActivity(int actIdIndex)
        {
            this.indentedActIdIndices.Add(actIdIndex);
        }

        public bool IsActivityIndented(int actIdIndex)
        {
            return this.indentedActIdIndices.Contains(actIdIndex);
        }


        public void UnIndentThread(int threadId)
        {
            this.indentedThreads.Remove(threadId);
        }

        public void UnIndentAll()
        {
            this.indentedThreads.Clear();
            this.indentedActIdIndices.Clear();
        }

        public void IndentThread(int threadId)
        {
            this.indentedThreads.Add(threadId);
        }

        public bool IsThreadIndented(int threadId)
        {
            return this.indentedThreads.Contains(threadId);
        }

        /// <summary>
        /// Tag or replace the color's filter.
        /// </summary>
        /// <param name="index">the tag index</param>
        /// <param name="filter">the filter</param>
        public void Tag(int index, IFilter filter)
        {
            this.Tags[index] = filter;
        }

        public void UnTag(int index)
        {
            this.Tags.Remove(index);
        }

        public bool IsTagged(int index)
        {
            return this.Tags.ContainsKey(index);
        }

        // True if the view is in a progress of something
        public bool IsInProgress { get; private set; }
        public ProgressItem CurrentProgress { get; private set; } = new ProgressItem("Ready");

        public Task FindAsync(IFilter filter, int currentIndex, bool direction, CancellationToken token)
        {
            return this.DoProgressAsync(t => this.Find(filter, currentIndex, direction, t), "Searching ...", token);
        }
        /// <summary>
        /// Find the next occurrence. Yielding progress from 0 to 100, if -1 is yielded, it means no result till the end of the current direction.
        /// Continue iterating the progress will search from the other end. If -2 is yielded, it means no result for all InternalItems.
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="currentIndex"></param>
        /// <param name="direction"></param>
        /// <returns>progress</returns>
        private IEnumerable<int> Find(IFilter filter, int currentIndex, bool direction, CancellationToken token)
        {
            int total = this.TotalCount;

            bool loopedBack = false;
            for (int i = 1; i < total; i++)
            {
                if (token.IsCancellationRequested) yield break;

                if ((i + 1) % 100 == 0)
                {
                    yield return (i + 1) * 100 / total;
                }

                int logicalIndex = (currentIndex + (direction ? i : -i) + total) % (total);
                if (!loopedBack && (direction ^ currentIndex < logicalIndex))
                {
                    loopedBack = true;
                    yield return -1;
                }

                int physicalIndex = this.GetPhysicalIndex(logicalIndex);
                var item = this.Source[physicalIndex];
                var template = this.Source.Templates[item.TemplateId];


                if (filter.Match(item, template))
                {
                    this.SelectedRowIndex = logicalIndex;
                    break;
                }
            }

            yield return 100;
        }


        public Task CountAsync(IFilter filter, CancellationToken token)
        {
            return this.DoProgressAsync(t => this.Count(filter, t), "Counting ...", token);
        }

        private IEnumerable<int> Count(IFilter filter, CancellationToken token)
        {
            yield return 0;

            int total = this.TotalCount;
            int count = 0;


            for (int i = 0; i < total; i++)
            {
                if ((i + 1) % 100 == 0)
                {
                    yield return (i + 1) * 100 / total;
                }

                int physicalIndex = this.GetPhysicalIndex(i);
                var item = this.Source[physicalIndex];
                var template = this.Source.Templates[item.TemplateId];

                if (filter.Match(item, template))
                {
                    count++;
                }
            }

            this.LastCountResult = count;

            yield return 100;
        }

        public int? LastCountResult { get; private set; }

        public Task DoStatisticsAsync(CancellationToken token)
        {
            return this.DoProgressAsync(this.DoStatistics, "Doing Statistics ...", token);
        }

        private IEnumerable<int> DoStatistics(CancellationToken token)
        {
            int totalCount = this.TotalCount;
            if (totalCount > 0)
            {
                int firstIndex = this.GetPhysicalIndex(0), lastIndex = this.GetPhysicalIndex(totalCount - 1);
                var stat = new FilteredViewStatistics();
                stat.SetFirstLast(this.Source[firstIndex], this.Source[lastIndex]);

                for (var i = 0; i < totalCount; i++)
                {
                    if ((i + 1) % 100 == 0)
                    {
                        yield return ((i + 1) * 100) / totalCount;
                    }

                    var index = this.GetPhysicalIndex(i);
                    stat.Sample(this.Source[index], this.Source.Templates[this.Source[index].TemplateId]);
                }

                this.Statistics = stat;
            }

            yield return 100;
        }

        #endregion

        #region Events

        public event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        public IFilteredView<T> CreateChild(IFilter filter)
        {
            return new FilteredView<T>(filter, this, this.Source);
        }

        public IReadOnlyList<IFilter> GroupFilters { get; protected set; }

        #endregion

        #region Display

        public ILogSource<T> Source { get; set; }

        public int? FirstDisplayedScrollingRowIndex { get; set; }
        public int? SelectedRowIndex { get; set; }

        public string Name { get; }

        public int TotalCount => this.ItemIndices?.Count ?? this.Source.Count;

        public T GetRowValue(int rowIndex)
        {
            if (rowIndex >= this.TotalCount) return null;
            int index = this.GetPhysicalIndex(rowIndex);
            return this.Source[index];
        }

        public object GetColumnValue(DataItemBase baseItem, int columnIndex)
        {
            ColumnInfoAttribute ci = this.ColumnInfos[columnIndex];

            // only process the tag column
            if (string.Equals(ci.Name, "Tag", StringComparison.Ordinal))
            {
                return this.Tags?.Where(kvp => kvp.Value.Match(baseItem, this.Source.Templates[baseItem.TemplateId])).Select(kvp => kvp.Key).ToList();
            }
            else
            {
                return this.Source.GetColumnValue(baseItem, columnIndex);
            }
        }

        public int GetPhysicalIndex(int logicalIndex)
        {
            return this.ItemIndices?[logicalIndex] ?? logicalIndex;
        }

        public IReadOnlyList<ColumnInfoAttribute> ColumnInfos => this.Source.ColumnInfos;
        public IReadOnlyList<PropertyInfo> PropertyInfos => this.Source.PropertyInfos;

        public IReadOnlyList<string> Templates => this.Source.Templates;
        public FilteredViewStatistics Statistics { get; private set; }

        public TimeSpan LastProgressUsedTime { get; private set; } = TimeSpan.Zero;
        #endregion

        #region Constructors

        internal FilteredView(IFilter filter, FilteredView<T> parent, ILogSource<T> source) : this(filter.Name)
        {
            this.Filter = filter;
            this.Parent = parent;
            this.Source = source;

            if (this.Parent != null)
            {
                this.ItemIndices = new List<int>();
            }
        }

        protected FilteredView(string name)
        {
            this.Name = name;
            //this.Source.ItemAdded += (s, e) =>
            //{
            //    this.OnItemAdded(e);
            //};

            this.StartLoading();
        }

        /*
        internal FilteredView(string name, FilteredView<T> parent, InnerGroupData groupData, ILogSource<T> Source) : this(name)
        {
            this.Parent = parent;
            // this.GroupData = groupData;
            // Change this to filter design.
            this.ItemIndices = groupData.InnerGroupIndexes;
            this.Source = Source;
            this.IsInitialized = true;
        }
        */

        #endregion

        #region Disposable

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                this.loadingTimer?.Dispose();
                this.loadingTimer = null;

                this.progressSemaphore?.Dispose();
                this.progressSemaphore = null;
            }
        }

        #endregion

        #region Properties and fields

        const int ReportInterval = 20;

        private int parentIndex = 0;

        private IDictionary<int, IFilter> Tags { get; } = new Dictionary<int, IFilter>();

        protected IFilter Filter { get; set; }

        private FilteredView<T> Parent { get; }

        private List<int> ItemIndices { get; }

        private readonly HashSet<int> indentedThreads = new HashSet<int>();
        private readonly HashSet<int> indentedActIdIndices = new HashSet<int>();

        #endregion

        #region Private methods

        private int loadingPeriodMilliseconds = 2000;
        private NonReentrantTimer loadingTimer;

        private void StartLoading()
        {
            this.loadingTimer = new NonReentrantTimer(
                (token) => this.DoProgressAsync(this.LoadingThread, "Loading ...", token),
                this.loadingPeriodMilliseconds,
                this.loadingPeriodMilliseconds);
        }

        private SemaphoreSlim progressSemaphore = new SemaphoreSlim(1, 1);

        private async Task DoProgressAsync(Func<CancellationToken, IEnumerable<int>> progressive, string progressName, CancellationToken token)
        {
            await this.progressSemaphore.WaitAsync(token);
            Stopwatch watch = Stopwatch.StartNew();

            try
            {
                foreach (var p in progressive(token))
                {
                    // if progress step from 0 to 100 directly, don't show the progress
                    if (p < 100 && p > 0)
                    {
                        this.IsInProgress = true;
                        this.CurrentProgress.Progress = 5 + p * 95 / 100;
                        this.CurrentProgress.ActionName = progressName;
                    }
                }
            }
            finally
            {
                if (this.IsInProgress)
                {
                    // only when real progress, set the used time.
                    this.IsInProgress = false;
                    this.LastProgressUsedTime = watch.Elapsed;
                }

                this.CurrentProgress.ActionName = "Ready";
                this.progressSemaphore.Release();
            }
        }

        protected virtual IEnumerable<int> LoadingThread(CancellationToken token)
        {
            int parentTotal = this.Parent.TotalCount;
            int lastParentIndex = this.parentIndex;
            int total = parentTotal - lastParentIndex;

            // filtered view
            while (this.parentIndex < parentTotal && !token.IsCancellationRequested)
            {
                var physicalIndex = this.Parent.GetPhysicalIndex(this.parentIndex);
                var item = this.Source[physicalIndex];
                var template = this.Source.Templates[item.TemplateId];

                if (this.Filter.Match(item, template))
                {
                    this.AddItem(physicalIndex);
                }

                this.parentIndex++;

                var step = this.parentIndex - lastParentIndex;
                if ((step % 100) == 0)
                {
                    yield return step * 100 / total;
                }
            }

            yield return 100;
        }

        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        private void AddItem(int index)
        {
            this.ItemIndices.Add(index);

            // passing the raw index directly to the child for performance.
            this.OnItemAdded(index);
        }

        #endregion
    }
}