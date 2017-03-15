namespace LogFlow.Viewer.LogFilter.Tokens
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text.RegularExpressions;

    internal static class Lexer
    {
        private const string Pattern = @"(!|(?:(?:\w:)?\""(?:[^\""]*(?:\""\"")*)*\"")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)"; // (!|(?:(?:\w:)?\"(?:[^\"]*(?:\"\")*)*\")|[()]|(?:&&)|(?:\|\|)|[^\s)]+)
        private static readonly Regex FilterStringRegex = new Regex(Pattern);

        internal static List<Token> Tokenize(string input)
        {
            MatchCollection tokenMatches = FilterStringRegex.Matches(input);
            var tokens = tokenMatches.Cast<Match>().Select(m => CreateToken(m, true)).ToList();
            tokens.Add(new EndOfFileToken { Index = input.Length });
            return tokens;
        }

        internal static List<SyntaxHighlightHint> GetHighlightHints(string input)
        {
            MatchCollection tokenMatches = FilterStringRegex.Matches(input);
            return tokenMatches.Cast<Match>()
                    .Select(m => CreateToken(m, false))
                    .Where(t => t != null)
                    .Select(t => t.HighlightHint).ToList();
        }

        internal static Token CreateToken(Match tokenMatch, bool throwEx)
        {
            if (string.IsNullOrWhiteSpace(tokenMatch.Value))
            {
                if (throwEx)
                {
                    throw new ArgumentException("Parsed string should not be null or empty");
                }
                else
                {
                    return null;
                }
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
                if (throwEx)
                {
                    throw new ParsingException("Failed to match double quotes.", tokenMatch.Index);
                }
                else
                {
                    return null;
                }
            }
            else
            {
                return new ContentToken(tokenMatch);
            }
        }
    }
}
