namespace LogFlow.DataModel
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.ComponentModel.Composition;

    public class LogSourceManager
    {
        private static readonly Lazy<LogSourceManager> LazyInstance = new Lazy<LogSourceManager>(() => new LogSourceManager());

        [ImportMany]
        private IEnumerable<Lazy<ILogSourceFactory<DataItemBase>, ILogSourceFactoryData>> logSourceFactories;

        private LogSourceManager()
        {

        }

        public ILogSource<DataItemBase> GetLogSource(string sourceInitializationData, LogSourceProperties properties, string extension)
        {
            // TODO: make the sources dynamic load.
            return new CosmosLogSource(sourceInitializationData, properties);
            //return new FakeLogSource();
        }

        public static LogSourceManager Instance => LazyInstance.Value;
    }
}
