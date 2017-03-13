namespace LogFilter.Expressions
{
    using System.Collections.Generic;
    using System.Globalization;

    using LogFilter.Tokens;

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

        internal TextMatchExpression(ContentToken token) : this(token.Content)
        {
        }

        private readonly HashSet<int> matchedTemplateIds = new HashSet<int>();

        internal string TextContent { get; set; }

        protected override string EvalToStringAcc() => $"\"{this.TextContent.Replace("\"", "\"\"")}\"";

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
    }
}
