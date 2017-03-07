namespace LogFilter.Tokens
{
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Text.RegularExpressions;

    internal class FilterStringParser
    {
        private const string Pattern = @"(!|(?:(?:\w:)?\""(?:[^\""]*(?:\""\"")*)*\"")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)"; // (!|(?:(?:\w:)?\"(?:[^\"]*(?:\"\")*)*\")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)
        private static readonly Regex FilterStringRegex = new Regex(Pattern);

        internal static List<string> ToTokenStrs(string filter)
        {
            var tokens = new List<string>();
            var matches = FilterStringRegex.Matches(filter);

            foreach (Match m in matches)
            {
                Debug.WriteLine("'{0}' found at index {1}.", m.Value, m.Index);
                tokens.Add(m.Value);
            }

            return tokens;
        }
    }
}
