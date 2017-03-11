namespace LogFilter.Expressions
{
    using System;

    using LogFilter.Tokens;

    internal class DatetimeEndMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeEndContent { get; set; }

        internal DatetimeEndMatchExpression(ContentToken token)
        {
            DateTime dt;
            string content = token.Content;
            if (!DateTime.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out dt))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(DateTime)}' format", token.Index);
            }
            this.DatetimeEndContent = dt;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.DatetimeEndContent:s}";

        public override bool Match<T>(T item, string template) => item.Time < this.DatetimeEndContent;

        internal static string ExpressionHeader => "e:";
    }
}
