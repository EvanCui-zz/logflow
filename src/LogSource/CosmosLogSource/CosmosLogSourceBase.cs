namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using LogFlow.DataModel.Algorithm;

    public abstract class CosmosLogSourceBase : LogSourceBase<CosmosDataItem>, IDisposable
    {
        protected Dictionary<IntPtr, int> TemplateMap = new Dictionary<IntPtr, int>();

        protected List<CosmosLogFileBase> LogFiles;

        public override string Name => this.LogFiles.Count == 1 ? this.LogFiles[0].FileName :
            (this.LogFiles.Count == 0 ? "No File Loaded" : $"{this.LogFiles[0].FileName} .. {this.LogFiles[this.LogFiles.Count - 1].FileName}");

        public override object GetColumnValue(int rowIndex, int columnIndex)
        {
            if (columnIndex == this.ColumnInfos.Count - 1)
            {
                return this.LogFiles[this.Items[rowIndex].FileIndex].FileName;
            }
            else
            {
                return base.GetColumnValue(rowIndex, columnIndex);
            }
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                this.LogFiles?.ForEach(f => f?.Dispose());
                this.LogFiles = null;
            }
        }

        public override IEnumerable<int> Load(IFilter filter)
        {
            int lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            var merged = HeapMerger.Merge(
                Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                this.LogFiles.Select(f => f.Where(i => filter?.Match(i.Item, i.Template) ?? true)).ToArray());

            //   var merged = this.LogFiles[0].Select(i => new MergedItem<FullCosmosDataItem>(i, 0));

            // each time we iterate through the merged, it will refresh all files underneath and load in new InternalItems;

            foreach (var item in merged)
            {
                this.AddItem(item.Item.Item);

                item.Item.Item.TemplateId = this.AddTemplate(item.Item.Template);

                //    var groupData = this.InnerGroupData[item.SourceIndex];
                //   groupData.Value.Percentage = (int)this.LogFiles[item.SourceIndex].GetPercent();

                //   groupData.Value.InnerGroupIndexes.Add(item.Item.Item.Id);
                item.Item.Item.FileIndex = item.SourceIndex;

                int totalPercent = (int)this.LogFiles.Average(f => f.GetPercent());
                if (totalPercent >= lastReportedProgress)
                {
                    yield return lastReportedProgress;
                    lastReportedProgress += 20;
                }
            }
        }
    }
}
