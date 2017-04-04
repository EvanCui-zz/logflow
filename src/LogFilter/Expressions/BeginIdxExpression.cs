namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class BeginIdxExpression : ContentMatchExpression
    {
        internal int BeginIdxContent { get; set; }

        internal BeginIdxExpression(ContentToken token)
        {
            this.BeginIdxContent = ContentParsingModule.ParseInt(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.BeginIdxContent}";

        public override bool Match<T>(T item, string template) => item.Id >= this.BeginIdxContent;

        internal static string ExpressionHeader => "bi:";
    }
}
