namespace LogFilter.Tokens
{
    internal class ContentToken : Token
    {
        internal string Content { get; }

        internal ContentToken(string str)
        {
            this.Content = str;
        }
    }
}
