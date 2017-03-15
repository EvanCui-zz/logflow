namespace LogFlow.Viewer.LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class LogicalOrToken : BinaryOperaterToken
    {
        internal LogicalOrToken(Match match) : base(match)
        {
        }
    }
}
