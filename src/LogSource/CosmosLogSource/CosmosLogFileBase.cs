namespace LogFlow.DataModel
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Collections.Concurrent;
    using System.Diagnostics;
    using System.Threading;
    using System.IO;

    /// <summary>
    /// The Log file Enumerable, node that this is a bit different from the standard enumerable, this enumerable cannot have multiple enumerators
    /// But it can only work when the last enumerator ends, and the new enumerator will check for file changes after that.
    /// </summary>
    public abstract class CosmosLogFileBase : IDisposable, IEnumerable<FullCosmosDataItem>
    {
        protected string FilePath;
        private bool isDisposed;
        public bool AutoLoadEnabled { get; set; }

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
                this.isDisposed = true;

                var reader = Interlocked.Exchange(ref this.Reader, null);
                reader?.Dispose();
            }
        }

        public IEnumerator<FullCosmosDataItem> GetEnumerator()
        {
            return new CosmosFileEnumerator(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        public double GetPercent() { return this.isDisposed ? 100 : this.Reader.GetPercent(); }

        protected BinaryLogReaderWrapperBase Reader;

        public class CosmosFileEnumerator : IEnumerator<FullCosmosDataItem>
        {
            private BlockingCollection<FullCosmosDataItem> itemQueue = new BlockingCollection<FullCosmosDataItem>();
            private readonly CosmosLogFileBase file;
            private CancellationTokenSource cts;
            private readonly Thread readingThread;

            public CosmosFileEnumerator(CosmosLogFileBase file)
            {
                this.file = file;

                if (this.file?.isDisposed ?? true)
                {
                    this.itemQueue.CompleteAdding();
                }
                else
                {
                    this.cts = new CancellationTokenSource();
                    this.readingThread = new Thread(() => this.ReadThread(this.cts.Token));
                    this.readingThread.Start();
                }
            }

            public void Dispose()
            {
                this.Dispose(true);
                GC.SuppressFinalize(this);
            }

            private void Dispose(bool isDisposing)
            {
                if (isDisposing)
                {
                    this.cts?.Cancel();
                    this.cts?.Dispose();
                    this.cts = null;

                    this.readingThread?.Join();

                    this.itemQueue?.Dispose();
                    this.itemQueue = null;
                }
            }

            private void ReadThread(CancellationToken token)
            {
                try
                {
                    if (!this.file.Reader.Refresh())
                    {
                        this.file.Dispose();
                        return;
                    }

                    if (token.IsCancellationRequested) return;

                    while (true)
                    {
                        var item = this.file.Reader.ReadItem();

                        if (item.Item == null || token.IsCancellationRequested)
                        {
                            break;
                        }

                        this.itemQueue.Add(item, token);
                    }
                }
                catch (OperationCanceledException ex)
                {
                    Debug.WriteLine($"Operation cancelled in reading thread. {ex}");
                }
                finally
                {
                    this.itemQueue.CompleteAdding();
                }
            }

            object IEnumerator.Current => this.Current;
            public FullCosmosDataItem Current { get; private set; }

            public bool MoveNext()
            {
                try
                {
                    this.Current = this.itemQueue.Take();
                    return true;
                }
                catch (InvalidOperationException)
                {
                    return false;
                }
            }

            public void Reset()
            {
                this.file.Reader.Reset();
            }
        }
    }
}
