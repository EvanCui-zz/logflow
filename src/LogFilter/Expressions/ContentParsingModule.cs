namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;
    using System.Linq;
    using LogFlow.Viewer.LogFilter.Tokens;

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

        internal static int[] ParseIntArray(string header, ContentToken token)
        {
            string[] arrayContent = token.Content.Substring(header.Length).Split(',');
            int[] res = arrayContent.Select(c =>
                {
                    int i;
                    if (!int.TryParse(c, out i))
                    {
                        throw new ParsingException($"{token.Content} is not of '{header}int1,int2,...' format",
                            token.Index);
                    }
                    else
                    {
                        return i;
                    }
                })
                .ToArray();

            return res;
        }
    }
}
