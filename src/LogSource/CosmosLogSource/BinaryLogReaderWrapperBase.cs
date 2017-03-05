using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public abstract class BinaryLogReaderWrapperBase : IDisposable
    {
        public abstract FullCosmosDataItem ReadItem();

        public abstract void Refresh();

        public abstract void Reset();

        public abstract double GetPercent();

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected abstract void Dispose(bool isDisposing);
    }
}
