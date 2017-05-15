namespace LogFlow.DataModel.LogSource
{
    using System.ComponentModel.Composition;

    [Export(typeof(ILogSourceFactory<DataItemBase>))]
    [ExportMetadata("LogFormat", ".bin")]
    public class CosmosLogSourceFactory : ILogSourceFactory<CosmosDataItem>
    {
        public ILogSource<CosmosDataItem> CreateInstance(string sourceInitializationData, LogSourceProperties properties)
        {
            return new CosmosLogSource(sourceInitializationData, properties);
        }
    }
}
