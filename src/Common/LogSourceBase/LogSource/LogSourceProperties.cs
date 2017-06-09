namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    public class LogSourceProperties
    {
        public LogSourceProperties(bool dynamicLoading, bool compress)
        {
            this.DynamicLoadingEnabled = dynamicLoading;
            this.CompressEnabled = compress;
        }

        public bool DynamicLoadingEnabled { get; }
        public bool CompressEnabled { get; }
    }
}
