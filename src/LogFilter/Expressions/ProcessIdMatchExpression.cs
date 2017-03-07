namespace LogFilter.Expressions
{
    using System;

    internal class ProcessIdMatchExpression : ContentMatchExpression
    {
        internal int ProcessIdContent { get; set; }

        internal ProcessIdMatchExpression(string content)
        {
            int i;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ArgumentException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format");
            }
            this.ProcessIdContent = i;
        }

        internal override string EvalToString() => $"{ExpressionHeader}{this.ProcessIdContent}";

        public override bool Match<T>(T item, string template) => item.ProcessId == this.ProcessIdContent;

        internal static string ExpressionHeader => "p:";
    }
}
