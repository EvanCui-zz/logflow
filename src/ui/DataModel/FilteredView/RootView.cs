namespace LogFlow.DataModel
{
    using System;

    public class RootView<T> : FilteredView<T>, IDisposable where T : DataItemBase
    {
        public RootView(ILogSource<T> logSource) : base(logSource.Name)
        {
            this.Data = logSource;
            this.GroupFilters = logSource.GroupFilters;

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
