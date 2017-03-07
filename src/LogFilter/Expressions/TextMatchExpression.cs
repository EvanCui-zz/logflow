namespace LogFilter.Expressions
{
    using System.Collections.Generic;
    using System.Globalization;

    internal class TextMatchExpression : ContentMatchExpression
    {
        internal TextMatchExpression(string content)
        {
            string str = content;
            if (str.StartsWith(ExpressionHeader))
            {
                str = str.Substring(ExpressionHeader.Length);
            }

            this.TextContent = this.RestoreEscaped(str);
        }

        private readonly HashSet<int> matchedTemplateIds = new HashSet<int>();

        internal string TextContent { get; set; }

        internal override string EvalToString() => $"\"{this.TextContent.Replace("\"", "\"\"")}\"";

        public override bool Match<T>(T item, string template)
        {
            string content = this.TextContent.ToUpperInvariant();

            if (this.matchedTemplateIds.Contains(item.TemplateId))
            {
                return true;
            }

            var text = template.ToUpperInvariant();

            //todo : improve the perf.
            if (text.Contains(content))
            {
                if (item.TemplateId != -1)
                {
                    this.matchedTemplateIds.Add(item.TemplateId);
                }

                return true;
            }
            else
            {
                text = string.Format(CultureInfo.InvariantCulture, template, item.Parameters).ToUpperInvariant();
                return text.Contains(content);
            }
        }

        internal static string ExpressionHeader => "c:";

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
