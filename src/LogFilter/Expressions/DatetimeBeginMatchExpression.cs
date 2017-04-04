namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class DatetimeBeginMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeBeginContent { get; set; }

        internal DatetimeBeginMatchExpression(ContentToken token)
        {
            this.DatetimeBeginContent = ContentParsingModule.ParseDateTime(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.DatetimeBeginContent:s}";

        public override bool Match<T>(T item, string template) => item.Time > this.DatetimeBeginContent;

        internal static string ExpressionHeader => "b:";
    }
}
