using System;
using LogFlow.Viewer.LogFilter.Tokens;

namespace LogFlow.Viewer.LogFilter.Expressions
{
    internal static class ContentParsingModule
    {
        internal static int ParseInt(string header, ContentToken token)
        {
            int i;
            string content = token.Content;
            if (!int.TryParse(content.Substring(header.Length).Trim('"'), out i))
            {
                throw new ParsingException($"{content} is not of '{header}{nameof(Int32)}' format", token.Index);
            }
            else
            {
                return i;
            }
        }

        internal static DateTime ParseDateTime(string header, ContentToken token)
        {
            DateTime dt;
            string content = token.Content;
            if (!DateTime.TryParse(content.Substring(header.Length).Trim('"'), out dt))
            {
                throw new ParsingException($"{content} is not of '{header}{nameof(DateTime)}' format", token.Index);
            }
            else
            {
                return dt;
            }
        }
    }
}
