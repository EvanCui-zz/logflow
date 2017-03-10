namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class LogicalAndToken : BinaryOperaterToken
    {
        public LogicalAndToken(Match match)
            : base(match)
        {
        }
    }
}
