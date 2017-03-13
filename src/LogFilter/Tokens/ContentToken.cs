namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal class ContentToken : Token
    {
        internal string Content { get; }

        internal ContentToken(string str)
        {
            this.Content = str;
        }

        internal ContentToken(Match match)
            : base(match)
        {
            this.Content = match.Value;
        }

        internal override TokenType Type => TokenType.Content;
    }
}
