using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    [ReadOnly(true)]
    public class FilteredViewStatistics
    {
        public int Errors { get; set; }
        public int Warnings { get; set; }
        public int Criticals { get; set; }
        public int TotalCount { get; set; }
        public int ExceptionCount { get { return this.Exceptions.Count; } }

        //TODO: get the correct editor.
        [Editor]
        public IList<string> Exceptions { get; set; } = new List<string>();
        public int Threads { get; set; }
        public int Processes { get; set; }
        public int Activities { get; set; }
        public int Files { get; set; }

        private HashSet<int> ThreadIds { get; set; } = new HashSet<int>();
        private HashSet<int> ProcessIds { get; set; } = new HashSet<int>();
        private HashSet<Guid> ActivityIds { get; set; } = new HashSet<Guid>();
        private HashSet<string> FileNames { get; set; } = new HashSet<string>();

        public void Sample<T>(T item, string template) where T : DataItemBase
        {
            this.TotalCount++;
            if (item.Level.HasFlag(LogLevel.Error)) { this.Errors++; }
            if (item.Level.HasFlag(LogLevel.Warning)) { this.Warnings++; }
            if (item.Level.HasFlag(LogLevel.Critical)) { this.Criticals++; }
            var text = string.Format(template, item.Parameters);
            if (text.ToLower().Contains("exception"))
            {
                this.Exceptions.Add(text);
            }

            if (this.ThreadIds.Add(item.ThreadId)) { this.Threads++; }

            if (this.ProcessIds.Add(item.ProcessId)) { this.Processes++; }
        }
    }
}
