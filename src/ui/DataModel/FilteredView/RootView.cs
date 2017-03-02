using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class RootView<T> : FilteredView<T>, IDisposable where T : DataItemBase
    {
        public RootView(string name, ILogSource<T> logSource, IFilter filter = null) : base(name)
        {
            this.Data = logSource;
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
                var data = this.Data as IDisposable;
                data?.Dispose();
                this.Data = null;
            }
        }
    }
}
