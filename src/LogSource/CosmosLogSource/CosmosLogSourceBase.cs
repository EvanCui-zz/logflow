using LogFlow.DataModel.Algorithm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public abstract class CosmosLogSourceBase : LogSourceBase<CosmosDataItem>, IDisposable
    {
        protected Dictionary<IntPtr, int> templateMap = new Dictionary<IntPtr, int>();

        protected List<CosmosLogFileBase> logFiles;

        public override string Name => this.logFiles.Count == 1 ? this.logFiles[0].FileName :
            (this.logFiles.Count == 0 ? "No File Loaded" : $"{this.logFiles[0].FileName} .. {this.logFiles[this.logFiles.Count - 1].FileName}");

        public override object GetColumnValue(int rowIndex, int columnIndex)
        {
            if (columnIndex == this.ColumnInfos.Count - 1)
            {
                return this.logFiles[this.Items[rowIndex].FileIndex].FileName;
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
                this.logFiles?.ForEach(f => f?.Dispose());
                this.logFiles = null;
            }
        }

        public override IEnumerable<int> Load(IFilter filter)
        {
            int lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            var merged = HeapMerger.Merge(
                Comparer<FullCosmosDataItem>.Create((d1, d2) => d1.Item.Time.CompareTo(d2.Item.Time)),
                this.logFiles.Select(f => f.Where(i => filter?.Match(i.Item, i.Template) ?? true)).ToArray());

            //   var merged = this.logFiles[0].Select(i => new MergedItem<FullCosmosDataItem>(i, 0));

            // each time we iterate through the merged, it will refresh all files underneath and load in new items;

            foreach (var item in merged)
            {
                this.AddItem(item.Item.Item);

                item.Item.Item.TemplateId = this.AddTemplate(item.Item.Template);

                //    var groupData = this.InnerGroupData[item.SourceIndex];
                //   groupData.Value.Percentage = (int)this.logFiles[item.SourceIndex].GetPercent();

                //   groupData.Value.InnerGroupIndexes.Add(item.Item.Item.Id);
                item.Item.Item.FileIndex = item.SourceIndex;

                int totalPercent = (int)this.logFiles.Average(f => f.GetPercent());
                if (totalPercent >= lastReportedProgress)
                {
                    yield return lastReportedProgress;
                    lastReportedProgress += 20;
                }
            }
        }
    }
}
