using System.Runtime.CompilerServices;

[assembly: InternalsVisibleTo("LogFilter.UnitTest")]

namespace LogFilter
{
    using LogFilter.Expressions;

    using LogFlow.DataModel;

    public class LogFilterInterpreter : IFilter
    {
        internal string FilterString { get; set; }

        private Expression Ast;

        public LogFilterInterpreter(string filterString)
        {
            this.FilterString = filterString;
            this.Parse();
        }

        public void Parse()
        {
            var tokens = Lexer.Tokenize(this.FilterString);
            ShuntingYardParser parser = new ShuntingYardParser(tokens);
            this.Ast = parser.Parse();
        }

        public string Name => this.Ast.Name;

        public bool Match<T>(T item, string template) where T : DataItemBase
        {
            return this.Ast.Match(item, template);
        }
    }
}

