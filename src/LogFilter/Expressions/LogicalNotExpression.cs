namespace LogFilter.Expressions
{
    internal class LogicalNotExpression : UnaryExpression
    {
        internal Expression Oprand { get; set; }

        internal override string EvalToString()
        {
            return $"!{this.Oprand.EvalToString()}";
        }

        public override bool Match<T>(T item, string template) => !this.Oprand.Match(item, template);
    }
}
