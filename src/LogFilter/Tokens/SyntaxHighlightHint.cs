namespace LogFilter.Tokens
{
    public enum TokenType
    {
        Keyword,
        Content,
        Eof
    }

    public class SyntaxHighlightHint
    {
        internal SyntaxHighlightHint(TokenType type, int index, int length)
        {
            this.Type = type;
            this.Index = index;
            this.Length = length;
        }

        internal SyntaxHighlightHint(Token token) : this(token.Type, token.Index, token.Length)
        {
        }

        public TokenType Type { get; }

        public int Index { get; }

        public int Length { get; }
    }
}
