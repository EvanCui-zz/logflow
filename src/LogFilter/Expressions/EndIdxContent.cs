namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class EndIdxExpression : ContentMatchExpression
    {
        internal int EndIdxContent { get; set; }

        internal EndIdxExpression(ContentToken token)
        {
            this.EndIdxContent = ContentParsingModule.ParseInt(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.EndIdxContent}";

        public override bool Match<T>(T item, string template) => item.Id <= this.EndIdxContent;

        internal static string ExpressionHeader => "ei:";
    }
}
