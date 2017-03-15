namespace LogFlow.Viewer.LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class CloseParenthesisToken : Token
    {
        internal CloseParenthesisToken(Match match) : base(match)
        {
        }

        internal override TokenType Type => TokenType.Keyword;
    }
}
