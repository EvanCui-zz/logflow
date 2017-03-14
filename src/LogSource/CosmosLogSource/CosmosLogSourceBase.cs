using System.Diagnostics;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Threading;
    using LogFlow.DataModel.Algorithm;

    public abstract class CosmosLogSourceBase : LogSourceCompressdBase<CosmosDataItem>, IDisposable
    {
        protected List<CosmosLogFileBase> LogFiles;

        public override string Name => this.LogFiles.Count == 1 ? this.LogFiles[0].FileName :
            (this.LogFiles.Count == 0 ? "No File Loaded" : $"{this.LogFiles[0].FileName} .. {this.LogFiles[this.LogFiles.Count - 1].FileName}");

        private bool isInProgress;
        private readonly CancellationTokenSource cts = new CancellationTokenSource();

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

        private void SetAutoLoading()
        {
            this.LogFiles.ForEach(f => f.AutoLoadEnabled = this.AutoLoadingEnabled);
        }

        public override IEnumerable<int> Peek(IFilter filter, int peekCount, CancellationToken token)
        {
            return this.CancellableProgress(t => this.PeekInternal(filter, peekCount, t), token);
        }

        public IEnumerable<int> PeekInternal(IFilter filter, int peekCount, CancellationToken token)
        {
            this.SetAutoLoading();
            this.isInProgress = true;
            var lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            int[] lastPercents = new int[this.LogFiles.Count];

            var merged = HeapMerger.Merge(
                token,
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
                    item.Item.Item.FileIndex = this.files.Put(this.LogFiles[item.SourceIndex].FileName);
                    this.AddItem(item.Item.Item);

                    yield break;
                }

                lastPercents[item.SourceIndex] = item.Item.Percent;

                var totalPercent = (int)lastPercents.Average();
                if (totalPercent < lastReportedProgress) continue;

                yield return lastReportedProgress;
                lastReportedProgress += 20;
            }
        }

        protected override IEnumerable<int> LoadFirst(IFilter filter, CancellationToken token)
        {
            return this.Load(filter, token);
        }

        protected override IEnumerable<int> LoadIncremental(IFilter filter, CancellationToken token)
        {
            if (this.AutoLoadingEnabled)
            {
                return this.Load(filter, token);
            }

            return base.LoadIncremental(filter, token);
        }

        public new IEnumerable<int> Load(IFilter filter, CancellationToken token)
        {
            return this.CancellableProgress(t => this.LoadInternal(filter, t), token);
        }

        private IEnumerable<int> LoadInternal(IFilter filter, CancellationToken token)
        {
            this.SetAutoLoading();
            var lastReportedProgress = 0;
            yield return lastReportedProgress;

            if (this.LogFiles.Count == 0) { yield return 100; yield break; }

            // 1 file split to 5 steps. n file to 5 * n.
            var reportInterval = Math.Max(1, 100 / (this.LogFiles.Count * 5));
            var firstReportCount = 100;
            int count = 0;

            lastReportedProgress += reportInterval;

            int[] lastPercents = new int[this.LogFiles.Count];

            IEnumerable<MergedItem<FullCosmosDataItem>> merged;
            if (filter == null)
            {
                merged = HeapMerger.Merge(
                    token,
                    Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                    this.LogFiles.Cast<IEnumerable<FullCosmosDataItem>>().ToArray());
            }
            else
            {
                merged = HeapMerger.Merge(
                    token,
                    Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                    this.LogFiles.Select(f => f.TakeWhile(i => !token.IsCancellationRequested)
                        .Where(i => filter.Match(i.Item, i.Template))).ToArray());
            }

            //   var merged = this.LogFiles[0].Select(i => new MergedItem<FullCosmosDataItem>(i, 0));

            // each time we iterate through the merged, it will refresh all files underneath and load in new InternalItems;
            foreach (var item in merged)
            {
                if (token.IsCancellationRequested) yield break;

                count++;
                item.Item.Item.TemplateId = this.AddTemplate(item.Item.Template);
                item.Item.Item.FileIndex = this.files.Put(this.LogFiles[item.SourceIndex].FileName);
                this.AddItem(item.Item.Item);

                //    var groupData = this.InnerGroupData[item.SourceIndex];
                //   groupData.Value.Percentage = (int)this.LogFiles[item.SourceIndex].GetPercent();

                //   groupData.Value.InnerGroupIndexes.Add(item.Item.Item.Id);

                lastPercents[item.SourceIndex] = item.Item.Percent;
                var totalPercent = (int)lastPercents.Average();
                if (totalPercent < lastReportedProgress && count != firstReportCount) continue;

                yield return lastReportedProgress;
                lastReportedProgress += reportInterval;
            }

            yield return 100;
        }
    }
}
