using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public abstract class CosmosLogSourceBase<T> : LogSourceBase<T> where T : DataItemBase
    {
        public override IEnumerable<int> Load(IFilter filter)
        {
            int lastReportedProgress = 0;
            yield return lastReportedProgress;
            lastReportedProgress += 5;

            while (true)
            {
                var tuple = this.LoadItem(filter);
                var item = tuple.Item1;
                var progress = tuple.Item2;

                if (item == null)
                {
                    yield break;
                }

                item.Id = ++this.currentId;
                this.AddItem(item);

                if (progress >= lastReportedProgress)
                {
                    yield return lastReportedProgress;
                    lastReportedProgress += 5;
                }
            }
        }

        // todo : refactor the template passing.
        protected abstract Tuple<T, int> LoadItem(IFilter filter);
    }
}
