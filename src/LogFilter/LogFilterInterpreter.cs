using System.Runtime.CompilerServices;

[assembly: InternalsVisibleTo("LogFilter.UnitTest")]

namespace LogFilter
{
    using System.Collections.Generic;

    using LogFilter.Expressions;
    using LogFilter.Tokens;

    using LogFlow.DataModel;

    public class LogFilterInterpreter
    {
        public static Expression Parse(string filterString)
        {
            var tokens = Lexer.Tokenize(filterString);
            ShuntingYardParser parser = new ShuntingYardParser(tokens);
            return parser.Parse();
        }

        public static FilterParseResult GetFilterParseResult(string filterString)
        {
            try
            {
                Expression ast = Parse(filterString);
                return new FilterParseResult(filterString, ast);
            }
            catch (ParsingException ex)
            {
                return new FilterParseResult(filterString, ex);
            }
        }

        public static List<SyntaxHighlightHint> GetHighlightHints(string filterString) => Lexer.GetHighlightHints(filterString);
    }
}

