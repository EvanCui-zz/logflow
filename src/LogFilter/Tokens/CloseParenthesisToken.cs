namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class CloseParenthesisToken : Token
    {
        internal CloseParenthesisToken(Match match) : base(match)
        {
        }
    }
}
