namespace LogFilter.Expressions
{
    using System;

    using LogFilter.Tokens;

    internal class ProcessIdMatchExpression : ContentMatchExpression
    {
        internal int ProcessIdContent { get; set; }

        internal ProcessIdMatchExpression(ContentToken token)
        {
            int i;
            string content = token.Content;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format", token.Index);
            }
            this.ProcessIdContent = i;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.ProcessIdContent}";

        public override bool Match<T>(T item, string template) => item.ProcessId == this.ProcessIdContent;

        internal static string ExpressionHeader => "p:";
    }
}
