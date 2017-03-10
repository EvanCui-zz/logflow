using System.Diagnostics;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.IO;

    /// <summary>
    /// The Log file Enumerable, node that this is a bit different from the standard enumerable, this enumerable cannot have multiple enumerators
    /// But it can only work when the last enumerator ends, and the new enumerator will check for file changes after that.
    /// </summary>
    public abstract class CosmosLogFileBase : IDisposable, IEnumerable<FullDataItemStruct>
    {
        protected string FilePath;

        public string FileName => Path.GetFileName(FilePath);

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                this.Reader?.Dispose();
            }
        }

        public IEnumerator<FullDataItemStruct> GetEnumerator()
        {
            return new CosmosFileEnumerator(this.Reader);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        public double GetPercent() { return this.Reader.GetPercent(); }

        protected BinaryLogReaderWrapperBase Reader;

        public class CosmosFileEnumerator : IEnumerator<FullDataItemStruct>
        {
            private readonly BinaryLogReaderWrapperBase reader;
            public CosmosFileEnumerator(BinaryLogReaderWrapperBase reader)
            {
                this.reader = reader;
                this.reader.Refresh();
            }

            public void Dispose() { }

            object IEnumerator.Current => this.Current;
            public FullDataItemStruct Current { get; private set; }

            public bool MoveNext()
            {
                try
                {
                    this.Current = this.reader.ReadItem();
                    return true;
                }
                catch (InvalidOperationException)
                {
                    Debug.WriteLine("Move next returned false because of InvalidOperationException");
                    return false;
                }
            }

            public void Reset()
            {
                this.reader.Reset();
            }
        }
    }
}
