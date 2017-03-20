using System.Diagnostics;
using System.Threading;

namespace LogFlow.DataModel
{
    using System;

    public class RootView<T> : FilteredView<T>, IDisposable where T : DataItemBase
    {
        private Timer internTimer;
        private bool isInInternProgress;

        public RootView(ILogSource<T> logSource, IFilter filter, bool backgroundIntern) : base(
            $"{logSource.Name}{(filter != null ? $" ({filter.Name})" : null)}{(logSource.Properties.DynamicLoadingEnabled ? " (AutoLoad)" : null)}{(logSource.Properties.CompressEnabled ? " (Compressed)" : null)}")
        {
            this.Source = logSource;
            this.Filter = filter;
            this.GroupFilters = logSource.GroupFilters;

            if (backgroundIntern)
            {
                this.internTimer = new Timer(this.BackGroundIntern, null, 5000, -1);
            }

            //if (logSource.GroupData != null)
            //{
            //    this.children = logSource.GroupData.Select(g => new FilteredView<T>(g.Key, this, g.Value, logSource)).Cast<IFilteredView<T>>().ToList();
            //}
        }

        private void BackGroundIntern(object state)
        {
            this.isInInternProgress = true;
            this.Source?.InternStrings();
            this.internTimer?.Change(20000, -1);
            this.isInInternProgress = false;
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
                while (this.isInInternProgress)
                {
                    Debug.WriteLine("SpinWait for string intern");
                    Thread.SpinWait(100);
                }

                this.internTimer?.Dispose();
                this.internTimer = null;

                var data = this.Source as IDisposable;
                data?.Dispose();
                this.Source = null;
            }
        }
    }
}
