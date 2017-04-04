namespace LogFlow.Viewer.LogFilter.Expressions
{
    using System;
    using System.Linq;

    using LogFlow.Viewer.LogFilter.Tokens;

    internal class ProcessIdMatchExpression : ContentMatchExpression
    {
        internal int[] ProcessIdsContent { get; set; }

        internal ProcessIdMatchExpression(ContentToken token)
        {
            this.ProcessIdsContent = ContentParsingModule.ParseIntArray(ExpressionHeader, token);
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{string.Join(",", this.ProcessIdsContent)}";

        public override bool Match<T>(T item, string template) => this.ProcessIdsContent.Contains(item.ProcessId);

        internal static string ExpressionHeader => "p:";
    }
}
