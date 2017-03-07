namespace LogFilter.Expressions
{
    internal class LogicalOrExpression : BinaryExpression
    {
        internal override string EvalToString()
        {
            return $"{this.Lhs.EvalToString()} || {this.Rhs.EvalToString()}";
        }

        public override bool Match<T>(T item, string template)
            => this.Lhs.Match(item, template) || this.Rhs.Match(item, template);
    }
}
