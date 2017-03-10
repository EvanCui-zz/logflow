namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class UnaryOperatorToken : Token
    {
        internal UnaryOperatorToken(Match match)
            : base(match)
        {
        }
    }
}
