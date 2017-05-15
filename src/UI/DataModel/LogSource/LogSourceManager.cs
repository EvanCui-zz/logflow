namespace LogFlow.DataModel
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.ComponentModel.Composition;
    using System.ComponentModel.Composition.Hosting;
    using System.Diagnostics;

    public class LogSourceManager
    {
        private static readonly Lazy<LogSourceManager> LazyInstance = new Lazy<LogSourceManager>(() => new LogSourceManager());

        private CompositionContainer container;

        [ImportMany]
        private IEnumerable<Lazy<ILogSourceFactory<DataItemBase>, ILogSourceFactoryData>> logSourceFactories;

        private LogSourceManager()
        {
            var catalog = new AggregateCatalog();
            catalog.Catalogs.Add(new AssemblyCatalog(typeof(LogSourceManager).Assembly));
            this.container = new CompositionContainer(catalog);

            try
            {
                this.container.ComposeParts(this);
            }
            catch (CompositionException ex)
            {
                Trace.TraceError(ex.ToString());
            }
        }

        public ILogSource<DataItemBase> GetLogSource(string sourceInitializationData, LogSourceProperties properties, string extension)
        {
            foreach (var factory in this.logSourceFactories)
            {
                if (factory.Metadata.LogFormat.Equals(extension, StringComparison.OrdinalIgnoreCase))
                {
                    return factory.Value.CreateInstance(sourceInitializationData, properties);
                }
            }

            throw new InvalidOperationException($"Extension name {extension} is not support by any LogSource plugin.");
        }

        public static LogSourceManager Instance => LazyInstance.Value;
    }
}
