namespace LogFlow.DataModel
{
    using System;

    public class LogSourceManager
    {
        private static readonly Lazy<LogSourceManager> LazyInstance = new Lazy<LogSourceManager>(() => new LogSourceManager());

        private LogSourceManager()
        {

        }

        public ILogSource<DataItemBase> GetLogSource(string sourceInitializationData, LogSourceProperties properties)
        {
            // TODO: make the sources dynamic load.
            return new CosmosLogSource(sourceInitializationData, properties);
            //return new FakeLogSource();
        }

        public static LogSourceManager Instance => LazyInstance.Value;
    }
}
