namespace LogFilter.Expressions
{
    internal abstract class BinaryExpression : Expression
    {
        internal Expression Lhs { get; set; }

        internal Expression Rhs { get; set; }

    }
}
