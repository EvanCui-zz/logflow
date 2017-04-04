namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class EndIdxExpression : ContentMatchExpression
    {
        internal int EndIdxContent { get; set; }

        internal EndIdxExpression(ContentToken token)
        {
            int i;
            string content = token.Content;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format", token.Index);
            }
            this.EndIdxContent = i;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.EndIdxContent}";

        public override bool Match<T>(T item, string template) => item.Id <= this.EndIdxContent;

        internal static string ExpressionHeader => "ei:";
    }
}
