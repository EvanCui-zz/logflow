using System.Runtime.CompilerServices;

[assembly: InternalsVisibleTo("LogFilter.UnitTest")]

namespace LogFilter
{
    using LogFilter.Expressions;

    using LogFlow.DataModel;

    public class LogFilterInterpreter
    {
        internal string FilterString { get; set; }

        internal Expression Ast { get; private set; }

        public static IFilter Parse(string filterString)
        {
            var tokens = Lexer.Tokenize(filterString);
            ShuntingYardParser parser = new ShuntingYardParser(tokens);
            return parser.Parse();
        }

        public string Name => this.Ast.Name;
    }
}

