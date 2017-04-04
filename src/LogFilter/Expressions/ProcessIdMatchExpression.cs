namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class ProcessIdMatchExpression : ContentMatchExpression
    {
        internal int ProcessIdContent { get; set; }

        internal ProcessIdMatchExpression(ContentToken token)
        {
            this.ProcessIdContent = ContentParsingModule.ParseInt(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.ProcessIdContent}";

        public override bool Match<T>(T item, string template) => item.ProcessId == this.ProcessIdContent;

        internal static string ExpressionHeader => "p:";
    }
}
