using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    /// <summary>
    /// The Log file Enumerable, node that this is a bit different from the standard enumerable, this enumerable cannot have multiple enumerators
    /// But it can only work when the last enumerator ends, and the new enumerator will check for file changes after that.
    /// </summary>
    public abstract class CosmosLogFileBase : IDisposable, IEnumerable<FullCosmosDataItem>
    {
        protected string filePath;

        public string FileName => Path.GetFileName(filePath);

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                this.reader?.Dispose();
            }
        }

        public IEnumerator<FullCosmosDataItem> GetEnumerator()
        {
            return new CosmosFileEnumerator(this.reader);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        public double GetPercent() { return this.reader.GetPercent(); }

        protected BinaryLogReaderWrapperBase reader;

        public class CosmosFileEnumerator : IEnumerator<FullCosmosDataItem>
        {
            private BinaryLogReaderWrapperBase reader;
            public CosmosFileEnumerator(BinaryLogReaderWrapperBase reader)
            {
                this.reader = reader;
                this.reader.Refresh();
            }

            public void Dispose() { }

            object IEnumerator.Current { get { return this.Current; } }
            public FullCosmosDataItem Current { get; private set; }

            public bool MoveNext()
            {
                this.Current = this.reader.ReadItem();
                return this.Current.Item != null;
            }

            public void Reset()
            {
                this.reader.Reset();
            }
        }
    }
}
