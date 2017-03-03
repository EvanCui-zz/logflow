using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LowFlow.DataModel;

namespace LogFlow.DataModel
{
    public abstract class CosmosLogSourceBase : LogSourceBase<CosmosDataItem>, IDisposable
    {
        protected Dictionary<IntPtr, int> templateMap = new Dictionary<IntPtr, int>();
        protected List<string> fileNames;

        public override object GetColumnValue(int rowIndex, int columnIndex)
        {
            if (columnIndex == this.ColumnInfos.Count - 1)
            {
                return this.fileNames[this.Items[rowIndex].FileIndex];
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

        protected abstract void Dispose(bool isDisposing);

        public override IEnumerable<int> Load(IFilter filter)
        {
            int lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 20;

            while (true)
            {
                var tuple = this.LoadItem(filter);
                var item = tuple.Item1;
                var progress = tuple.Item2;

                if (item == null)
                {
                    yield break;
                }

                this.AddItem(item);
                this.InnerGroupIndexes[item.FileIndex].Value.Add(item.Id);

                if (progress >= lastReportedProgress)
                {
                    yield return lastReportedProgress;
                    lastReportedProgress += 20;
                }
            }
        }

        // todo : refactor the template passing.
        protected abstract Tuple<CosmosDataItem, int> LoadItem(IFilter filter);
    }
}
