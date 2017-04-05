namespace LogFlow.Viewer.LogFilter
{
    using Expressions;

    public class FilterParseResult
    {
        public FilterParseResult(string input, Expression ast)
        {
            this.Succeed = true;
            this.Input = input;
            this.Ast = ast;
        }

        public FilterParseResult(string input, int columnIndex, string errorMessage)
        {
            this.Succeed = false;
            this.Input = input;
            this.ColumnIndex = columnIndex;
            this.ErrorMessage = errorMessage;
        }

        public FilterParseResult(string input, ParsingException ex)
            : this(input, ex.ColumnIndex, ex.Message)
        {
        }

        public bool Succeed { get; }

        public Expression Ast { get; }

        public int ColumnIndex { get; }

        public string Input { get; }

        public string ErrorMessage { get; }
    }
}