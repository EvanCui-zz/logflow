namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal abstract class BinaryOperaterToken : Token
    {
        internal BinaryOperaterToken(Match match)
            : base(match)
        {
        }
    }
}
