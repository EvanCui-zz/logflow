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
        public RootView(ILogSource<T> logSource, IFilter filter = null) : base(logSource.Name)
        {
            this.Data = logSource;

            if (logSource.GroupFilters != null)
            {
                this.children = logSource.GroupFilters.Select(f => this.CreateChild(f)).ToList();
            }
            //if (logSource.GroupData != null)
            //{
            //    this.children = logSource.GroupData.Select(g => new FilteredView<T>(g.Key, this, g.Value, logSource)).Cast<IFilteredView<T>>().ToList();
            //}
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
