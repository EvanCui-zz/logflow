namespace LogFilter.Tokens
{
    using System.Text.RegularExpressions;

    internal abstract class Token
    {
        internal Token()
        {
        }

        internal Token(Match tokenMatch)
        {
            this.Index = tokenMatch.Index;
            this.Length = tokenMatch.Length;
        }

        internal int Index { get; }

        internal int Length { get; }
    }
}
