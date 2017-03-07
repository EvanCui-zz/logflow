namespace LogFilter.Expressions
{
    using System;

    internal class DatetimeBeginMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeBeginContent { get; set; }

        internal DatetimeBeginMatchExpression(string content)
        {
            DateTime dt;
            if (!DateTime.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out dt))
            {
                throw new ArgumentException($"{content} is not of '{ExpressionHeader}{nameof(DateTime)}' format");
            }
            this.DatetimeBeginContent = dt;
        }

        internal override string EvalToString() => $"b:{this.DatetimeBeginContent:s}";

        public override bool Match<T>(T item, string template) => item.Time > this.DatetimeBeginContent;

        internal static string ExpressionHeader => "b:";
    }
}
