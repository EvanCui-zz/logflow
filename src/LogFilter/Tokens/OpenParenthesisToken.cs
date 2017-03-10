namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class OpenParenthesisToken : Token
    {
        internal OpenParenthesisToken()
        {
        }

        internal OpenParenthesisToken(Match match) : base(match)
        {
        }
    }
}
