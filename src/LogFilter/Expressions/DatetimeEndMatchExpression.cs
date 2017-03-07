namespace LogFilter.Expressions
{
    using System;

    internal class DatetimeEndMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeEndContent { get; set; }

        internal DatetimeEndMatchExpression(string content)
        {
            DateTime dt;
            if (!DateTime.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out dt))
            {
                throw new ArgumentException($"{content} is not of '{ExpressionHeader}{nameof(DateTime)}' format");
            }
            this.DatetimeEndContent = dt;
        }

        internal override string EvalToString() => $"{ExpressionHeader}{this.DatetimeEndContent:s}";

        public override bool Match<T>(T item, string template) => item.Time < this.DatetimeEndContent;

        internal static string ExpressionHeader => "e:";
    }
}
