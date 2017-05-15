namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System.Collections.Generic;
    using System.Globalization;

    using LogFlow.Viewer.LogFilter.Tokens;

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
            this.UpperCaseTextContent = this.TextContent.ToUpperInvariant();
        }

        internal TextMatchExpression(ContentToken token) : this(token.Content)
        {
        }

        private readonly HashSet<int> matchedTemplateIds = new HashSet<int>();

        internal string TextContent { get; }

        internal string UpperCaseTextContent { get; }

        protected override string EvalToStringAcc() => $"\"{this.TextContent.Replace("\"", "\"\"")}\"";

        public override bool Match<T>(T item, string template)
        {
            if (this.matchedTemplateIds.Contains(item.TemplateId))
            {
                return true;
            }

            var text = template.ToUpperInvariant();

            //todo : improve the perf.
            if (text.Contains(this.UpperCaseTextContent))
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
                return text.Contains(this.UpperCaseTextContent);
            }
        }

        internal static string ExpressionHeader => "c:";
    }
}
