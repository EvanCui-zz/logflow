namespace LogFlow.Viewer.LogFilter.Expressions
{
    internal class ParenthesisedExpression : UnaryExpression
    {
        internal Expression Oprand { get; set; }

        internal override string EvalToString(bool showOrder)
        {
            return $"({this.Oprand.EvalToString(showOrder)})";
        }

        public override bool Match<T>(T item, string template) => this.Oprand.Match(item, template);
    }
}
