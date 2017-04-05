namespace LogFlow.Viewer.LogFilter.Expressions
{
    internal class LogicalOrExpression : BinaryExpression
    {
        protected override string EvalToStringAcc(bool showOrder)
        {
            return $"{this.Lhs.EvalToString(showOrder)} || {this.Rhs.EvalToString(showOrder)}";
        }

        public override bool Match<T>(T item, string template)
            => this.Lhs.Match(item, template) || this.Rhs.Match(item, template);
    }
}
