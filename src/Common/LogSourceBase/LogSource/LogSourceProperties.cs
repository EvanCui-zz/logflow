namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    public class LogSourceProperties
    {
        public LogSourceProperties(bool dynamicLoading, bool compress, bool internStrings, int internIntervalMilliseconds)
        {
            this.InternStrings = internStrings;
            this.DynamicLoadingEnabled = dynamicLoading;
            this.CompressEnabled = compress;
            this.InternIntervalMilliseconds = internIntervalMilliseconds;
        }

        public bool InternStrings { get; }
        public int InternIntervalMilliseconds { get; }
        public bool DynamicLoadingEnabled { get; }
        public bool CompressEnabled { get; }
    }
}
