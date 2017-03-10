namespace LogFilter
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Linq;
    using System.Text.RegularExpressions;

    using LogFilter.Tokens;

    internal static class Lexer
    {
        private const string Pattern = @"(!|(?:(?:\w:)?\""(?:[^\""]*(?:\""\"")*)*\"")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)"; // (!|(?:(?:\w:)?\"(?:[^\"]*(?:\"\")*)*\")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)
        private static readonly Regex FilterStringRegex = new Regex(Pattern);

        internal static List<Token> Tokenize(string input)
        {
            MatchCollection tokenMatches = FilterStringRegex.Matches(input);
            return tokenMatches.Cast<Match>().Select(CreateToken).ToList();
        }

        internal static Token CreateToken(Match tokenMatch)
        {
            if (string.IsNullOrWhiteSpace(tokenMatch.Value))
            {
                throw new ArgumentException("Parsed string should not be null or empty");
            }
            else if (tokenMatch.Value == "&&")
            {
                return new LogicalAndToken(tokenMatch);
            }
            else if (tokenMatch.Value == "||")
            {
                return new LogicalOrToken(tokenMatch);
            }
            else if (tokenMatch.Value == "(")
            {
                return new OpenParenthesisToken(tokenMatch);
            }
            else if (tokenMatch.Value == ")")
            {
                return new CloseParenthesisToken(tokenMatch);
            }
            else if (tokenMatch.Value == "!")
            {
                return new LogicalNotToken(tokenMatch);
            }
            else if (tokenMatch.Value == "\"")
            {
                throw new ParsingException("Failed to match double quotes.", tokenMatch.Index);
            }
            else
            {
                return new ContentToken(tokenMatch);
            }
        }
    }
}
