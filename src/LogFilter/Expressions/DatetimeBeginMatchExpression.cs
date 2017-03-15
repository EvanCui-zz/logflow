namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class DatetimeBeginMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeBeginContent { get; set; }

        internal DatetimeBeginMatchExpression(ContentToken token)
        {
            DateTime dt;
            string content = token.Content;
            if (!DateTime.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out dt))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(DateTime)}' format", token.Index);
            }
            this.DatetimeBeginContent = dt;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.DatetimeBeginContent:s}";

        public override bool Match<T>(T item, string template) => item.Time > this.DatetimeBeginContent;

        internal static string ExpressionHeader => "b:";
    }
}
