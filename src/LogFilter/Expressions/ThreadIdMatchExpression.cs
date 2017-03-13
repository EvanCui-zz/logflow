namespace LogFilter.Expressions
{
    using System;

    using LogFilter.Tokens;

    internal class ThreadIdMatchExpression : ContentMatchExpression
    {
        internal int ThreadIdContent { get; set; }

        internal ThreadIdMatchExpression(ContentToken token)
        {
            int i;
            string content = token.Content;
            if (!int.TryParse(content.Substring(ExpressionHeader.Length).Trim('"'), out i))
            {
                throw new ParsingException($"{content} is not of '{ExpressionHeader}{nameof(Int32)}' format", token.Index);
            }
            this.ThreadIdContent = i;
        }

        protected override string EvalToStringAcc() => $"{ExpressionHeader}{this.ThreadIdContent}";

        public override bool Match<T>(T item, string template) => item.ThreadId == this.ThreadIdContent;

        internal static string ExpressionHeader => "t:";
    }
}
