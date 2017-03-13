using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;

    public class FilteredView<T> : IFilteredView<T> where T : DataItemBase
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

        public void UnIndentThread(int threadId)
        {
            this.indentedThreads.Remove(threadId);
        }

        public void UnIndentAll()
        {
            this.indentedThreads.Clear();
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

        /// <summary>
        /// Find the next occurrence. Yielding progress from 0 to 100, if -1 is yielded, it means no result till the end of the current direction.
        /// Continue iterating the progress will search from the other end. If -2 is yielded, it means no result for all InternalItems.
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="currentIndex"></param>
        /// <param name="direction"></param>
        /// <returns>progress</returns>
        public IEnumerable<int> Find(IFilter filter, int currentIndex, bool direction)
        {
            if (!this.IsInitialized || this.IsInProgress) yield break;

            this.OnReportStart("Searching");
            yield return 0;

            int total = this.ItemIndices?.Count ?? this.Data.Count;

            bool loopedBack = false;
            for (int i = 1; i < total; i++)
            {
                if (i % (total / 20) == 0)
                {
                    int progress = 5 + ((i * 100) / total) * 90 / 100;
                    yield return progress;
                    this.OnReportProgress(progress);
                }

                int logicalIndex = (currentIndex + (direction ? i : -i) + total) % (total);
                if (!loopedBack && (direction ^ currentIndex < logicalIndex))
                {
                    loopedBack = true;
                    yield return -1;
                }

                int index = this.GetPhysicalIndex(logicalIndex);

                if (filter.Match(this.Data[index], this.Data.Templates[this.Data[index].TemplateId]))
                {
                    this.SelectedRowIndex = logicalIndex;
                    break;
                }
            }

            yield return 100;
            this.OnReportFinish();
        }

        public IEnumerable<int> Count(IFilter filter)
        {
            if (!this.IsInitialized || this.IsInProgress) yield break;

            this.OnReportStart("Counting");
            yield return 0;

            int total = this.TotalCount;
            int count = 0;

            for (int i = 0; i < total; i++)
            {
                if (i % (total / 20) == 0)
                {
                    int progress = 5 + ((i * 100) / total) * 90 / 100;
                    yield return progress;
                    this.OnReportProgress(progress);
                }

                int index = this.GetPhysicalIndex(i);

                if (filter.Match(this.Data[index], this.Data.Templates[this.Data[index].TemplateId]))
                {
                    count++;
                }
            }

            this.LastCountResult = count;

            yield return 100;
            this.OnReportFinish();
        }

        public int? LastCountResult { get; private set; }

        public IEnumerable<int> Initialize(bool statistics, CancellationToken token)
        {
            if (this.IsInitialized || this.IsInProgress) yield break;

            this.OnReportStart("Initializing");
            yield return 5;

            const int reportInterval = 20;

            if (this.Parent == null)
            {
                foreach (var progress in this.Data.Load(this.Filter, token))
                {
                    var p = 5 + progress * 65 / 100;
                    this.OnReportProgress(p);
                    yield return p;
                }

                this.Data.ItemAdded += (s, e) =>
                {
                    this.OnItemAdded(e);
                };
            }
            else
            {
                if (!this.Parent.IsInitialized)
                {
                    // don't initialize the local view when its parent is still not initialized.
                    this.IsInitialized = false;

                    yield return 100;
                    this.OnReportFinish();

                    yield break;
                }

                var total = this.Parent.TotalCount;

                var reportIndex = Math.Max(total / reportInterval, 2);

                for (var i = 0; i < total; i++)
                {
                    if (i % reportIndex == 0)
                    {
                        var progress = 5 + ((i * 100) / total) * 65 / 100;
                        yield return progress;
                        this.OnReportProgress(progress);
                    }

                    var index = this.Parent.GetPhysicalIndex(i);

                    if (this.Filter.Match(this.Data[index], this.Data.Templates[this.Data[index].TemplateId]))
                    {
                        // we don't use this.AddItem here, to avoid flushing many events when initialize.
                        // only initialized is true, we fire events.
                        this.ItemIndices.Add(index);
                    }
                }
            }

            if (statistics && this.TotalCount > 0)
            {
                int firstIndex = this.GetPhysicalIndex(0), lastIndex = this.GetPhysicalIndex(this.TotalCount - 1);
                this.Statistics.SetFirstLast(this.Data[firstIndex], this.Data[lastIndex]);

                var reportIndex = Math.Max(this.TotalCount / reportInterval, 2);
                for (var i = 0; i < this.TotalCount; i++)
                {
                    if (i % reportIndex == 0)
                    {
                        var progress = ((i * 100) / this.TotalCount) * 30 / 100 + 70;
                        yield return progress;

                        this.OnReportProgress(progress);
                    }

                    var index = this.GetPhysicalIndex(i);
                    this.Statistics.Sample(this.Data[index], this.Data.Templates[this.Data[index].TemplateId]);
                }
            }

            this.IsInitialized = true;

            yield return 100;
            this.OnReportFinish();
        }

        public bool IsInitialized { get; private set; }

        #endregion

        #region Events

        public event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        public IFilteredView<T> CreateChild(IFilter filter)
        {
            return new FilteredView<T>(filter, this, this.Data);
        }

        public IReadOnlyList<IFilter> GroupFilters { get; protected set; }

        #endregion

        #region Display

        public event EventHandler<ProgressItem> ProgressChanged;
        public int? FirstDisplayedScrollingRowIndex { get; set; }
        public int? SelectedRowIndex { get; set; }

        public string Name { get; }

        public int TotalCount => this.ItemIndices?.Count ?? this.Data.Count;

        public T GetRowValue(int rowIndex)
        {
            if (rowIndex >= this.TotalCount) return null;
            int index = this.GetPhysicalIndex(rowIndex);
            return this.Data[index];
        }

        public object GetColumnValue(DataItemBase baseItem, int columnIndex)
        {
            ColumnInfoAttribute ci = this.ColumnInfos[columnIndex];

            // only process the tag column
            if (string.Equals(ci.Name, "Tag", StringComparison.Ordinal))
            {
                return this.Tags?.Where(kvp => kvp.Value.Match(baseItem, this.Data.Templates[baseItem.TemplateId])).Select(kvp => kvp.Key).ToList();
            }
            else
            {
                return this.Data.GetColumnValue(baseItem, columnIndex);
            }
        }

        public int GetPhysicalIndex(int logicalIndex)
        {
            return this.ItemIndices?[logicalIndex] ?? logicalIndex;
        }

        public IReadOnlyList<ColumnInfoAttribute> ColumnInfos => this.Data.ColumnInfos;
        public IReadOnlyList<PropertyInfo> PropertyInfos => this.Data.PropertyInfos;

        public IReadOnlyList<string> Templates => this.Data.Templates;
        public FilteredViewStatistics Statistics { get; } = new FilteredViewStatistics();

        #endregion

        #region Constructors

        internal FilteredView(IFilter filter, FilteredView<T> parent, ILogSource<T> data) : this(filter.Name)
        {
            this.Filter = filter;
            this.Parent = parent;
            this.Data = data;

            if (this.Parent != null)
            {
                this.ItemIndices = new List<int>();

                this.Parent.ItemAdded += (s, e) =>
                {
                    if (this.IsInitialized && filter.Match(this.Data[e], this.Data.Templates[this.Data[e].TemplateId]))
                    {
                        this.AddItem(e);
                    }
                };
            }
        }

        protected FilteredView(string name)
        {
            this.Name = name;
        }

        /*
        internal FilteredView(string name, FilteredView<T> parent, InnerGroupData groupData, ILogSource<T> data) : this(name)
        {
            this.Parent = parent;
            // this.GroupData = groupData;
            // Change this to filter design.
            this.ItemIndices = groupData.InnerGroupIndexes;
            this.Data = data;
            this.IsInitialized = true;
        }
        */

        #endregion

        #region Properties and fields

        //   private InnerGroupData GroupData { get; set; }

        private IDictionary<int, IFilter> Tags { get; } = new Dictionary<int, IFilter>();

        protected IFilter Filter { get; set; }

        private FilteredView<T> Parent { get; }

        internal ILogSource<T> Data { get; set; }

        private List<int> ItemIndices { get; }

        private readonly HashSet<int> indentedThreads = new HashSet<int>();

        #endregion

        #region Private methods

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

        private void OnReportStart(string actionName)
        {
            this.IsInProgress = true;
            this.CurrentProgress = new ProgressItem(actionName);
            this.OnReport(this.CurrentProgress);
        }

        private void OnReportFinish()
        {
            this.IsInProgress = false;
            this.CurrentProgress = new ProgressItem("Ready") { Progress = 100 };
            this.OnReport(this.CurrentProgress);
        }

        private void OnReportProgress(int progress)
        {
            this.CurrentProgress.Progress = progress;
            this.OnReport(this.CurrentProgress);
        }

        private void OnReport(ProgressItem item)
        {
            this.ProgressChanged?.Invoke(this, item);
        }

        #endregion
    }
}