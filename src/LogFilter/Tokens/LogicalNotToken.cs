namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class LogicalNotToken : UnaryOperatorToken
    {
        internal LogicalNotToken(Match match)
            : base(match)
        {
        }
    }
}
