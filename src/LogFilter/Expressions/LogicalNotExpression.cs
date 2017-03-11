namespace LogFilter.Expressions
{
    internal class LogicalNotExpression : UnaryExpression
    {
        internal Expression Oprand { get; set; }

        internal override string EvalToString(bool showOrder)
        {
            var res = $"!{this.Oprand.EvalToString(showOrder)}";
            if (showOrder)
            {
                return $"({res})";
            }
            else
            {
                return res;
            }
        }

        public override bool Match<T>(T item, string template) => !this.Oprand.Match(item, template);
    }
}
