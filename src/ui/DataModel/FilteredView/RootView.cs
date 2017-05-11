using System.Diagnostics;
using System.Threading;

namespace LogFlow.DataModel
{
    using System;

    public class RootView<T> : FilteredView<T> where T : DataItemBase
    {
        public RootView(ILogSource<T> logSource, IFilter filter) : base(
            $"{logSource.Name}{(filter != null ? $" ({filter.Name})" : null)}{(logSource.Properties.DynamicLoadingEnabled ? " (AutoLoad)" : null)}{(logSource.Properties.CompressEnabled ? " (Compressed)" : null)}")
        {
            this.Source = logSource;
            this.Filter = filter;
            this.GroupFilters = logSource.GroupFilters;
        }

        protected override void Dispose(bool isDisposing)
        {
            if (isDisposing)
            {
                var data = this.Source as IDisposable;
                data?.Dispose();
                this.Source = null;
            }

            base.Dispose(isDisposing);
        }
    }
}
