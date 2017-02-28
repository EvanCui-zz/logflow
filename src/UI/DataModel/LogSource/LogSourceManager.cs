using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class LogSourceManager
    {
        private static Lazy<LogSourceManager> instance = new Lazy<LogSourceManager>(() => new LogSourceManager());

        private LogSourceManager()
        {

        }

        public ILogSource<DataItemBase> GetLogSource(string sourceInitializationData)
        {
            return new FakeLogSource(true);
        }

        public static LogSourceManager Instance { get { return instance.Value; } }
    }
}
