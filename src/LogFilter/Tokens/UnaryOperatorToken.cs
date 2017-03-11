namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class UnaryOperatorToken : OperatorToken
    {
        internal UnaryOperatorToken(Match match)
            : base(match)
        {
        }

        internal override int Precedence() => 3;
    }
}
