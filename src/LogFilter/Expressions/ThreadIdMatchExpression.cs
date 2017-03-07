namespace LogFilter.Expressions
{
    using System;

    internal class ThreadIdMatchExpression : ContentMatchExpression
    {
        internal int ThreadIdContent { get; set; }

        internal ThreadIdMatchExpression(string content)
        {
            int i;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ArgumentException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format");
            }
            this.ThreadIdContent = i;
        }

        internal override string EvalToString() => $"{ExpressionHeader}{this.ThreadIdContent}";

        public override bool Match<T>(T item, string template) => item.ThreadId == this.ThreadIdContent;

        internal static string ExpressionHeader => "t:";
    }
}
