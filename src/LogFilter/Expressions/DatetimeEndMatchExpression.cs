namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class DatetimeEndMatchExpression : ContentMatchExpression
    {
        internal DateTime DatetimeEndContent { get; set; }

        internal DatetimeEndMatchExpression(ContentToken token)
        {
            this.DatetimeEndContent = ContentParsingModule.ParseDateTime(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.DatetimeEndContent:s}";

        public override bool Match<T>(T item, string template) => item.Time < this.DatetimeEndContent;

        internal static string ExpressionHeader => "e:";
    }
}
