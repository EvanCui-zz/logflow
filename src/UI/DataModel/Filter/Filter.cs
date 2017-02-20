using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace DataModel
{
    public class Filter
    {
        private readonly Regex FilterPattern = new Regex(
            @"(?<name>b|begin|e|end|t|thread|p|process|l|level|c|content):(?<value>[^\s""]+|""[^""]*"")|(?<content>[^\s]+)", RegexOptions.IgnoreCase | RegexOptions.ExplicitCapture);

        private Dictionary<char, LogLevel> LogLevelMap = new Dictionary<char, LogLevel>()
        {
            {'c', LogLevel.Critical },
            {'e', LogLevel.Error },
            {'w', LogLevel.Warning },
            {'i', LogLevel.Info },
            {'v', LogLevel.Verbose },
        };

        public Filter(string pattern)
        {
            this.Name = pattern;
            foreach (Match match in this.FilterPattern.Matches(pattern))
            {
                var name = match.Groups["name"].Value.ToLowerInvariant();
                var value = match.Groups["value"].Value.ToLowerInvariant();
                var content = match.Groups["content"].Value;

                if (string.IsNullOrEmpty(name))
                {
                    name = "c";
                    value = content;
                }

                switch (name[0])
                {
                    case 'b':
                    case 'e':
                        DateTime dt;
                        if (!DateTime.TryParse(value.Trim('"'), out dt))
                        {
                            throw new InvalidOperationException($"The pattern '{name}:{value}' is not of '{nameof(DateTime)}' format");
                        }
                        else
                        {
                            if (name[0] == 'e') this.End = dt;
                            else this.Begin = dt;
                        }

                        break;
                    case 't':
                    case 'p':
                        int intValue;
                        if (!int.TryParse(value, out intValue))
                        {
                            throw new InvalidOperationException($"The pattern '{name}:{value}' is not of '{nameof(Int32)}' format");
                        }
                        else
                        {
                            if (name[0] == 't') this.ThreadIds.Value.Add(intValue);
                            else if (name[0] == 'p') this.ProcessIds.Value.Add(intValue);
                        }

                        break;

                    case 'l':
                        this.Level = LogLevel.None;
                        foreach (char c in value)
                        {
                            LogLevel level;

                            if (!this.LogLevelMap.TryGetValue(c, out level))
                            {
                                throw new InvalidOperationException($"The pattern '{name}:{value}' is not of '{nameof(LogLevel)}' format");
                            }
                            else
                            {
                                this.Level |= level;
                            }
                        }

                        break;

                    case 'c':
                        this.Texts.Value.Add(value.Trim('"').ToUpperInvariant());
                        break;

                    default:
                        Debug.Assert(false, $"the pattern name '{name}' is unknown");
                        break;
                }
            }
        }

        public string Name { get; private set; }

        public DateTime? Begin { get; set; }
        public DateTime? End { get; set; }
        public LogLevel? Level { get; set; }

        public Lazy<HashSet<int>> ThreadIds { get; set; } = new Lazy<HashSet<int>>();
        public Lazy<HashSet<int>> ProcessIds { get; set; } = new Lazy<HashSet<int>>();
        public Lazy<HashSet<string>> Texts { get; set; } = new Lazy<HashSet<string>>();

        public bool Match<T>(T item, string template) where T : DataItemBase
        {
            if (this.Level.HasValue && !this.Level.Value.HasFlag(item.Level))
            {
                return false;
            }

            if (this.ThreadIds.IsValueCreated)
            {
                if (!this.ThreadIds.Value.Contains(item.ThreadId))
                {
                    return false;
                }
            }

            if (this.ProcessIds.IsValueCreated)
            {
                if (!this.ProcessIds.Value.Contains(item.ProcessId))
                {
                    return false;
                }
            }

            if (this.Begin.HasValue && item.Time < this.Begin)
            {
                return false;
            }

            if (this.End.HasValue && item.Time > this.End)
            {
                return false;
            }


            if (this.Texts.IsValueCreated)
            {
                var text = template.ToUpperInvariant();
                //todo : improve the perf.
                if (this.Texts.Value.All(t => text.Contains(t)))
                {
                    return true;
                }
                else
                {
                    text = string.Format(CultureInfo.InvariantCulture, template, item.Parameters).ToUpperInvariant();
                    return this.Texts.Value.All(t => text.Contains(t));
                }
            }

            return true;
        }
    }
}
