using System.Linq;

namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class ThreadIdMatchExpression : ContentMatchExpression
    {
        internal int[] ThreadIdsContent { get; set; }

        internal ThreadIdMatchExpression(ContentToken token)
        {
           this.ThreadIdsContent = ContentParsingModule.ParseIntArray(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{string.Join(",", this.ThreadIdsContent)}";

        public override bool Match<T>(T item, string template) => this.ThreadIdsContent.Contains(item.ThreadId);

        internal static string ExpressionHeader => "t:";
    }
}
