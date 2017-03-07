namespace LogFilter
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    using LogFilter.Tokens;

    internal static class Lexer
    {
        internal static List<Token> Tokenize(string input)
        {
            var strs = FilterStringParser.ToTokenStrs(input);
            return strs.Where(t => !string.IsNullOrWhiteSpace(t)).Select(CreateToken).ToList<Token>();
        }

        internal static Token CreateToken(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
            {
                throw new ArgumentException("Parsed string should not be null or empty");
            }
            else if (input == "&&")
            {
                return new LogicalAndToken();
            }
            else if (input == "||")
            {
                return new LogicalOrToken();
            }
            else if (input == "(")
            {
                return new OpenParenthesisToken();
            }
            else if (input == ")")
            {
                return new CloseParenthesisToken();
            }
            else if (input == "!")
            {
                return new LogicalNotToken();
            }
            else if (input == "\"")
            {
                throw new ArgumentException("Failed to match double quotes.");
            }
            else
            {
                return new ContentToken(input);
            }
        }
    }
}
