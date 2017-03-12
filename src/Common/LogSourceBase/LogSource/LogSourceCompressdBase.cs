namespace LogFlow.DataModel
{
    using Algorithm;
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    public abstract class LogSourceCompressdBase<T> : LogSourceBase<T> where T : DataItemBase, new()
    {
        private DateTime? baseTime;
        private IdentifierCache<int> threadIds = new IdentifierCache<int>();
        private IdentifierCache<int> processIds = new IdentifierCache<int>();

        public override int Count => this.CompressedItems8.Count;

        public override T this[int index]
        {
            get
            {
                var compressed = this.CompressedItems8[index];
                if (compressed.IsCompressed)
                {
                    var item = new T();
                    item.DeCompress8(compressed, this.baseTime.Value);
                    item.ProcessId = this.processIds[item.ProcessId];
                    item.ThreadId = this.threadIds[item.ThreadId];
                    item.Id = index;
                    item.Parameters = this.Parameters[index];
                    return item;
                }

                return base[compressed.Index];
            }
        }

        private List<CompressedDataItem8> CompressedItems8 = new List<CompressedDataItem8>();

        protected override void AddItem(T item)
        {
            if (!this.baseTime.HasValue) this.baseTime = item.Time;

            item.ProcessId = this.processIds.Put(item.ProcessId);
            item.ThreadId = this.threadIds.Put(item.ThreadId);            

            CompressedDataItem8 compressed;

            if (item.Compress8(this.baseTime.Value, out compressed))
            {
                compressed.IsCompressed = true;
            }

            if (!compressed.IsCompressed)
            {
                // link;
                this.InternalItems.Add(item);
                int internalIndex = this.InternalItems.Count - 1;
                compressed.Index = internalIndex;
            }

            this.CompressedItems8.Add(compressed);
            int index = this.CompressedItems8.Count - 1;
            this.Parameters.Add(item.Parameters);
            item.Id = index;

            Debug.Assert(index == this.Parameters.Count - 1, "compressed item list doesn't match parameters list");
            this.OnItemAdded(index);
        }
    }
}
