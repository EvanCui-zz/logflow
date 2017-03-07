namespace LogFilter.Tokens
{
    internal class ContentToken : Token
    {
        internal string Content { get; }

        internal ContentToken(string str)
        {
            this.Content = this.RestoreEscaped(str);
        }

        private string RestoreEscaped(string str)
        {
            if (str.StartsWith("\"") && str.EndsWith("\""))
            {
                return str.Substring(1, str.Length - 2).Replace("\"\"", "\"");
            }
            else
            {
                return str;
            }
        }
    }
}
