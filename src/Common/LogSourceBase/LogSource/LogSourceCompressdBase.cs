namespace LogFlow.DataModel
{
    using Algorithm;
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;

    public abstract class LogSourceCompressdBase<T> : LogSourceBase<T> where T : DataItemBase, new()
    {
        protected class FileCompressMetaData
        {
            public DateTime? BaseTime { get; set; }
            public IdentifierCache<int> ThreadIds { get; } = new IdentifierCache<int>();
            public IdentifierCache<int> ProcessIds { get; } = new IdentifierCache<int>();
            public IdentifierCache<Guid> ActivityIds { get; } = new IdentifierCache<Guid>();

            // todo put template in
        }

        protected readonly IdentifierCache<FileCompressMetaData> FileMetaData = new IdentifierCache<FileCompressMetaData>();
     //   private readonly FileCompressMetaData metaData = new FileCompressMetaData();

        public override int Count => this.CompressedItems8.Count;
        public override int Tier1Count => this.CompressedItems16.Count;
        public override int Tier2Count => this.InternalItems.Count;

        private FileCompressMetaData GetFileMetaData(int fileIndex)
        {
            //return this.metaData;
            return this.FileMetaData[fileIndex];
        }

        public override T this[int index]
        {
            get
            {
                if (!this.CompressionEnabled)
                {
                    return base[index];
                }

                var compressed = this.CompressedItems8[index];
                switch (compressed.State)
                {
                    case CompressState.Compressed8:
                        {
                            var item = new T();
                            var meta = this.GetFileMetaData(compressed.FileIndex);
                            item.DeCompress(compressed, meta.BaseTime.Value);
                            item.ProcessId = meta.ProcessIds[item.ProcessId];
                            item.ThreadId = meta.ThreadIds[item.ThreadId];
                            // todo: Aid
                            item.Id = index;
                            item.Parameters = this.Parameters[index];
                            return item;
                        }

                    case CompressState.Compressed16:
                        {
                            var item = new T();
                            var compress16 = this.CompressedItems16[compressed.Index];
                            var meta = this.GetFileMetaData(compress16.FileIndex);
                            item.DeCompress(compress16, meta.BaseTime.Value);
                            item.ProcessId = meta.ProcessIds[item.ProcessId];
                            item.ThreadId = meta.ThreadIds[item.ThreadId];

                            // todo: Aid
                            item.Id = index;
                            item.Parameters = this.Parameters[index];

                            return item;
                        }

                    case CompressState.NotCompressed:
                        return base[compressed.Index];
                    default:
                        return default(T);
                }
            }
        }

        private readonly List<CompressedDataItem8> CompressedItems8 = new List<CompressedDataItem8>();
        private readonly List<CompressedDataItem16> CompressedItems16 = new List<CompressedDataItem16>();

        protected override void AddItem(FullDataItem<T> item)
        {
            var meta = this.GetFileMetaData(item.Item.FileIndex);
            item.Item.TemplateId = this.AddTemplate(item.Template);

            if (!meta.BaseTime.HasValue) meta.BaseTime = item.Item.Time;

            item.Item.ProcessId = meta.ProcessIds.Put(item.Item.ProcessId);
            item.Item.ThreadId = meta.ThreadIds.Put(item.Item.ThreadId);
            item.Item.ActivityIdIndex = meta.ActivityIds.Put(item.ActivityId);

            CompressedDataItem8 compressed;

            if (this.CompressionEnabled)
            {
                if (!item.Item.Compress(meta.BaseTime.Value, out compressed))
                {
                    CompressedDataItem16 compressed16;
                    if (item.Item.Compress(meta.BaseTime.Value, out compressed16))
                    {
                        compressed16.State = CompressState.Compressed16;
                        this.CompressedItems16.Add(compressed16);

                        compressed.State = CompressState.Compressed16;
                        compressed.Index = this.CompressedItems16.Count - 1;
                    }
                    else
                    {
                        this.InternalItems.Add(item.Item);
                        compressed.State = CompressState.NotCompressed;
                        compressed.Index = this.InternalItems.Count - 1;
                    }
                }

                this.CompressedItems8.Add(compressed);
                var index = this.CompressedItems8.Count - 1;
                this.Parameters.Add(item.Item.Parameters);
                item.Item.Id = index;

                Debug.Assert(index == this.Parameters.Count - 1, "compressed item list doesn't match parameters list");
            }
            else
            {
                this.InternalItems.Add(item.Item);
                item.Item.Id = this.InternalItems.Count - 1;
            }

            this.OnItemAdded(item.Item.Id);
        }
    }
}
