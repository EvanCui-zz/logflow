using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
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

        public List<int> Timeline { get; set; } = new List<int>(SplitCount);

        private HashSet<int> ThreadIds { get; set; } = new HashSet<int>();
        private HashSet<int> ProcessIds { get; set; } = new HashSet<int>();
        private HashSet<Guid> ActivityIds { get; set; } = new HashSet<Guid>();
        private HashSet<string> FileNames { get; set; } = new HashSet<string>();

        private const int SplitCount = 5;
        private double TimeSplitRange;
        private DateTime FirstTime;
        public void SetFirstLast<T>(T first, T last) where T : DataItemBase
        {
            this.TimeSplitRange = (last.Time - first.Time).TotalMilliseconds / SplitCount;
            this.FirstTime = first.Time;
        }

        public void Sample<T>(T item, string template) where T : DataItemBase
        {
            int chunk = 0;
            double chunkDouble = (item.Time - this.FirstTime).TotalMilliseconds / this.TimeSplitRange;
            if (!(double.IsInfinity(chunkDouble) || double.IsNaN(chunkDouble)))
            {
                chunk = (int)chunkDouble;
                if (chunk >= SplitCount) chunk = SplitCount - 1;
            } 

            while (this.Timeline.Count <= chunk) { this.Timeline.Add(0); }

            this.Timeline[chunk]++;

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
