namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class OpenParenthesisToken : OperatorToken
    {
        internal OpenParenthesisToken()
        {
        }

        internal OpenParenthesisToken(Match match) : base(match)
        {
        }

        internal override int Precedence() => 0;
    }
}
