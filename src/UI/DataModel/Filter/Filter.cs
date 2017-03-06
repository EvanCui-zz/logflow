namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Globalization;
    using System.Linq;
    using System.Text.RegularExpressions;

    public class Filter : IFilter
    {
        private readonly Regex FilterPattern = new Regex(
            @"(?<name>b|begin|e|end|t|thread|p|process|l|level|c|content):(?<value>[^\s""]+|""[^""]*"")|(?<content>[^\s""]+|""[^""]*"")", RegexOptions.IgnoreCase | RegexOptions.ExplicitCapture);

        private readonly Dictionary<char, LogLevels> LogLevelMap = new Dictionary<char, LogLevels>()
        {
            {'c', LogLevels.Critical },
            {'e', LogLevels.Error },
            {'w', LogLevels.Warning },
            {'i', LogLevels.Info },
            {'v', LogLevels.Verbose },
        };

        private readonly HashSet<int> matchedTemplateIds = new HashSet<int>();

        public static IFilter CreateFilter(Func<DataItemBase, string, bool> predicate)
        {
            return new Filter() { Predicate = predicate };
        }

        private Func<DataItemBase, string, bool> Predicate { get; set; }

        private Filter() { }

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
                        this.Level = LogLevels.None;
                        foreach (char c in value)
                        {
                            LogLevels level;

                            if (!this.LogLevelMap.TryGetValue(c, out level))
                            {
                                throw new InvalidOperationException($"The pattern '{name}:{value}' is not of '{nameof(LogLevels)}' format");
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

        public string Name { get; }

        public DateTime? Begin { get; set; }
        public DateTime? End { get; set; }
        public LogLevels? Level { get; set; }

        public Lazy<HashSet<int>> ThreadIds { get; set; } = new Lazy<HashSet<int>>();
        public Lazy<HashSet<int>> ProcessIds { get; set; } = new Lazy<HashSet<int>>();
        public Lazy<HashSet<string>> Texts { get; set; } = new Lazy<HashSet<string>>();

        public virtual bool Match<T>(T item, string template) where T : DataItemBase
        {
            if (this.Predicate != null) return this.Predicate(item, template);

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
                if (this.matchedTemplateIds.Contains(item.TemplateId))
                {
                    return true;
                }

                var text = template.ToUpperInvariant();

                //todo : improve the perf.
                if (this.Texts.Value.All(t => text.Contains(t)))
                {
                    if (item.TemplateId != -1)
                    {
                        this.matchedTemplateIds.Add(item.TemplateId);
                    }

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
