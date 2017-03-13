namespace LogFilter.Expressions
{
    internal abstract class BinaryExpression : Expression
    {
        internal Expression Lhs { get; set; }

        internal Expression Rhs { get; set; }

        internal override string EvalToString(bool showOrder)
        {
            string res = this.EvalToStringAcc(showOrder);
            if (showOrder)
            {
                return $"({res})";
            }
            else
            {
                return res;
            }
        }

        protected abstract string EvalToStringAcc(bool showOrder);

    }
}
