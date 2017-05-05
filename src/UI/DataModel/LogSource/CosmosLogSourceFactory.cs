using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel.LogSource
{
    [Export(typeof(ILogSourceFactory<DataItemBase>))]
    [ExportMetadata("LogFormat", "bin")]
    public class CosmosLogSourceFactory : ILogSourceFactory<CosmosDataItem>
    {
        public ILogSource<CosmosDataItem> CreateInstance(string sourceInitializationData, LogSourceProperties properties)
        {
            return new CosmosLogSource(sourceInitializationData, properties);
        }
    }
}
