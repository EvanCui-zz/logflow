namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class BeginIdxExpression : ContentMatchExpression
    {
        internal int BeginIdxContent { get; set; }

        internal BeginIdxExpression(ContentToken token)
        {
            int i;
            string content = token.Content;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format", token.Index);
            }
            this.BeginIdxContent = i;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.BeginIdxContent}";

        public override bool Match<T>(T item, string template) => item.Id >= this.BeginIdxContent;

        internal static string ExpressionHeader => "bi:";
    }
}
