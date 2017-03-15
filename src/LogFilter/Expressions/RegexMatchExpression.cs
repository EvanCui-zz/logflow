namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System.Globalization;
    using System.Text.RegularExpressions;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class RegexMatchExpression : ContentMatchExpression
    {
        internal RegexMatchExpression(string content)
        {
            string str = content;
            if (str.StartsWith(ExpressionHeader))
            {
                str = str.Substring(ExpressionHeader.Length);
            }

            this.RegexContent = new Regex(this.RestoreEscaped(str));
        }

        internal RegexMatchExpression(ContentToken token) : this(token.Content)
        {
        }

        internal Regex RegexContent { get; set; }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}\"{this.RegexContent.ToString().Replace("\"", "\"\"")}\"";

        public override bool Match<T>(T item, string template)
            => this.RegexContent.IsMatch(string.Format(CultureInfo.InvariantCulture, template, item.Parameters));

        internal static string ExpressionHeader => "r:";
    }
}
