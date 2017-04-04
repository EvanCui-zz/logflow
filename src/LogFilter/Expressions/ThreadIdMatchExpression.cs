namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class ThreadIdMatchExpression : ContentMatchExpression
    {
        internal int ThreadIdContent { get; set; }

        internal ThreadIdMatchExpression(ContentToken token)
        {
           this.ThreadIdContent = ContentParsingModule.ParseInt(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.ThreadIdContent}";

        public override bool Match<T>(T item, string template) => item.ThreadId == this.ThreadIdContent;

        internal static string ExpressionHeader => "t:";
    }
}
