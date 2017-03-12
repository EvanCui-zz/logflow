namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal abstract class BinaryOperaterToken : OperatorToken
    {
        internal BinaryOperaterToken()
        {
        }

        internal BinaryOperaterToken(Match match)
            : base(match)
        {
        }

        internal override int Precedence() => 2;

        internal override int NewPushPrecedence() => this.Precedence() - 1;
    }
}
