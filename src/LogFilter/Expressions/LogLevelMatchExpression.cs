namespace LogFilter.Expressions
{
    using System;
    using System.Collections.Generic;
    using System.Text;

    using LogFilter.Tokens;

    using LogFlow.DataModel;

    internal class LogLevelMatchExpression : ContentMatchExpression
    {
        private static readonly Dictionary<char, LogLevels> LogLevelMap = new Dictionary<char, LogLevels>()
        {
                { 'c', LogLevels.Critical },
                { 'e', LogLevels.Error },
                { 'w', LogLevels.Warning },
                { 'i', LogLevels.Info },
                { 'v', LogLevels.Verbose },
        };

        internal LogLevels Levels { get; set; }

        internal LogLevelMatchExpression(ContentToken token)
        {
            this.Levels = LogLevels.None;
            var content = token.Content;
            foreach (char c in content.Substring(ExpressionHeader.Length).Trim('"'))
            {
                LogLevels level;

                if (!LogLevelMap.TryGetValue(c, out level))
                {
                    throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(LogLevels)}' format", token.Index);
                }
                else
                {
                    this.Levels |= level;
                }
            }
        }

        internal override string EvalToString() => $"{ExpressionHeader}{ConstructShortForm(this.Levels)}";

        public override bool Match<T>(T item, string template) => this.Levels.HasFlag(item.Level);

        internal static string ExpressionHeader => "l:";

        private static string ConstructShortForm(LogLevels logLevels)
        {
            StringBuilder sb = new StringBuilder();
            if (logLevels.HasFlag(LogLevels.Critical))
            {
                sb.Append('c');
            }
            if (logLevels.HasFlag(LogLevels.Error))
            {
                sb.Append('e');
            }
            if (logLevels.HasFlag(LogLevels.Warning))
            {
                sb.Append('w');
            }
            if (logLevels.HasFlag(LogLevels.Info))
            {
                sb.Append('i');
            }
            if (logLevels.HasFlag(LogLevels.Verbose))
            {
                sb.Append('v');
            }

            return sb.ToString();
        }
    }
}
