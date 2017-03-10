using System.Linq;
using System.Text.RegularExpressions;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;

    [ReadOnly(true)]
    public class FilteredViewStatistics
    {
        private readonly Regex exceptionRegex = new Regex("(?<name>[\\w]*Exception)", RegexOptions.Compiled);
        public int Errors { get; set; }
        public int Warnings { get; set; }
        public int Criticals { get; set; }
        public int TotalCount { get; set; }
        public int ExceptionCount => this.Exceptions.Sum(kvp => kvp.Value);

        [Browsable(false)]
        public IDictionary<string, int> Exceptions { get; } = new Dictionary<string, int>();
        public int Threads { get; set; }
        public int Processes { get; set; }
        public int Activities { get; set; }
        public int Files { get; set; }

        [Browsable(false)]
        public List<int> Timeline { get; set; } = new List<int>(SplitCount);

        private HashSet<int> ThreadIds { get; } = new HashSet<int>();
        private HashSet<int> ProcessIds { get; } = new HashSet<int>();
        //private HashSet<Guid> ActivityIds { get; set; } = new HashSet<Guid>();
        //private HashSet<string> FileNames { get; set; } = new HashSet<string>();

        private const int SplitCount = 5;
        private double TimeSplitRange;
        private DateTime FirstTime;
        public void SetFirstLast(DataItemStruct first, DataItemStruct last)
        {
            this.TimeSplitRange = (last.Time - first.Time).TotalMilliseconds / SplitCount;
            this.FirstTime = first.Time;
        }

        public void Sample(DataItemStruct item, string template)
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
            if (item.Level.HasFlag(LogLevels.Error)) { this.Errors++; }
            if (item.Level.HasFlag(LogLevels.Warning)) { this.Warnings++; }
            if (item.Level.HasFlag(LogLevels.Critical)) { this.Criticals++; }
            var text = string.Format(template, item.Parameters);

            // Perf critical
            if (text.Contains("Exception"))
            {
                var m = this.exceptionRegex.Match(text);
                if (m.Success)
                {
                    var ex = m.Groups["name"].Value;
                    int count;
                    if (this.Exceptions.TryGetValue(ex, out count))
                    {
                        this.Exceptions[ex] = count + 1;
                    }
                    else
                    {
                        this.Exceptions[ex] = 1;
                    }
                }
            }

            if (this.ThreadIds.Add(item.ThreadId)) { this.Threads++; }

            if (this.ProcessIds.Add(item.ProcessId)) { this.Processes++; }
        }
    }
}
