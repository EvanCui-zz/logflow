namespace LogFilter.Expressions
{
    internal abstract class ContentMatchExpression : UnaryExpression
    {
        internal static ContentMatchExpression CreateContentMatchExpression(string content)
        {
            return new TextMatchExpression(content);
        }

        internal abstract string ExpressionHeader { get; }

        internal virtual bool MatchExpressionType(string content) => content.StartsWith(this.ExpressionHeader);
    }
}
