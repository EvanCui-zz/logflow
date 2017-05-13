namespace LogFlow.DataModel
{
    using System;

    public abstract class BinaryLogReaderWrapperBase : IDisposable
    {
        public abstract FullCosmosDataItem ReadItem();

        public abstract bool Refresh();

        public abstract void Reset();

        public abstract double GetPercent();

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            // We should keep this method as virtual but not abstract.
            // It should be a bug of managed C++, to be verified.
            // The managed C++ will override the Dispose(bool) method and call base.Dispose() from that method, which will cause dead loop.
            // When this method is virtual, the managed C++ generate new virtual Dispose(bool) method, so that the base.Dispose() won't call to that method again.
        }
    }
}
