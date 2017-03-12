namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal abstract class OperatorToken : Token
    {
        internal OperatorToken()
        {
        }

        internal OperatorToken(Match match)
            : base(match)
        {
        }

        internal override TokenType Type => TokenType.Keyword;

        internal abstract int Precedence();

        internal virtual int NewPushPrecedence() => this.Precedence();
    }
}
