using System.Diagnostics;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;
    using LogFlow.DataModel.Algorithm;

    public abstract class CosmosLogSourceBase : LogSourceBase<CosmosDataItem>, IDisposable
    {
        protected Dictionary<IntPtr, int> TemplateMap = new Dictionary<IntPtr, int>();

        protected List<CosmosLogFileBase> LogFiles;

        public override string Name => this.LogFiles.Count == 1 ? this.LogFiles[0].FileName :
            (this.LogFiles.Count == 0 ? "No File Loaded" : $"{this.LogFiles[0].FileName} .. {this.LogFiles[this.LogFiles.Count - 1].FileName}");

        public override object GetColumnValue(int rowIndex, int columnIndex)
        {
            return string.Equals(this.ColumnInfos[columnIndex].Name, "File", StringComparison.Ordinal) ?
                this.LogFiles[this.Items[rowIndex].FileIndex].FileName
                : base.GetColumnValue(rowIndex, columnIndex);
        }

        private bool isInProgress;
        private CancellationTokenSource cts = new CancellationTokenSource();

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                this.cts?.Cancel();
                while (this.isInProgress) { Debug.WriteLine("SpinWait dispose!"); Thread.SpinWait(100); }
                Debug.WriteLine("CosmosLogSource disposed");
                this.cts?.Dispose();
                this.LogFiles?.ForEach(f => f?.Dispose());
                this.LogFiles = null;
            }
        }

        private IEnumerable<int> CancellableProgress(Func<CancellationToken, IEnumerable<int>> action,
            CancellationToken token)
        {
            var currentCts = CancellationTokenSource.CreateLinkedTokenSource(token, this.cts.Token);
            token = currentCts.Token;

            try
            {
                this.isInProgress = true;
                foreach (var p in action(token))
                {
                    yield return p;
                }
            }
            finally
            {
                this.isInProgress = false;
                currentCts.Dispose();
            }
        }

        public override IEnumerable<int> Peek(IFilter filter, int peekCount, CancellationToken token)
        {
            return this.CancellableProgress(t => this.PeekInternal(filter, peekCount, t), token);
        }

        public IEnumerable<int> PeekInternal(IFilter filter, int peekCount, CancellationToken token)
        {
            this.isInProgress = true;
            var lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            var merged = HeapMerger.Merge(
                Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                this.LogFiles.Cast<IEnumerable<FullCosmosDataItem>>().ToArray());

            var count = 0;

            foreach (var item in merged)
            {
                if (token.IsCancellationRequested) yield break;
                if (++count > peekCount && peekCount >= 0)
                {
                    yield break;
                }

                if (filter.Match(item.Item.Item, item.Item.Template))
                {
                    item.Item.Item.TemplateId = this.AddTemplate(item.Item.Template);
                    item.Item.Item.FileIndex = item.SourceIndex;
                    this.AddItem(item.Item.Item);

                    yield break;
                }

                var totalPercent = (int)this.LogFiles.Average(f => f.GetPercent());
                if (totalPercent < lastReportedProgress) continue;

                yield return lastReportedProgress;
                lastReportedProgress += 20;
            }
        }

        public override IEnumerable<int> Load(IFilter filter, CancellationToken token)
        {
            return this.CancellableProgress(t => this.LoadInternal(filter, t), token);
        }

        private IEnumerable<int> LoadInternal(IFilter filter, CancellationToken token)
        {
            var lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            var filteredMerged = HeapMerger.Merge(Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                this.LogFiles.Select(f => f.Where(i =>
                {
                    // the loop should exit immediately when cancel.
                    token.ThrowIfCancellationRequested();
                    return filter?.Match(i.Item, i.Template) ?? true;
                })).ToArray());

            //   var merged = this.LogFiles[0].Select(i => new MergedItem<FullCosmosDataItem>(i, 0));

            // each time we iterate through the merged, it will refresh all files underneath and load in new InternalItems;
            foreach (var item in filteredMerged)
            {
                if (token.IsCancellationRequested) yield break;

                item.Item.Item.TemplateId = this.AddTemplate(item.Item.Template);
                item.Item.Item.FileIndex = item.SourceIndex;
                this.AddItem(item.Item.Item);

                //    var groupData = this.InnerGroupData[item.SourceIndex];
                //   groupData.Value.Percentage = (int)this.LogFiles[item.SourceIndex].GetPercent();

                //   groupData.Value.InnerGroupIndexes.Add(item.Item.Item.Id);

                var totalPercent = (int)this.LogFiles.Average(f => f.GetPercent());
                if (totalPercent < lastReportedProgress) continue;

                yield return lastReportedProgress;
                lastReportedProgress += 20;
            }
        }
    }
}
