namespace LogFilter.Expressions
{
    using System;
    using System.Collections.Generic;

    internal abstract class ContentMatchExpression : UnaryExpression
    {
        internal static ContentMatchExpression CreateContentMatchExpression(string content)
        {
            string header = content.Substring(0, content.IndexOf(':') + 1);
            if (string.IsNullOrWhiteSpace(header))
            {
                return new TextMatchExpression(content);
            }
            else
            {
                Func<string, ContentMatchExpression> ctor;
                if (Ctors.TryGetValue(header, out ctor))
                {
                    return ctor(content);
                }
                else
                {
                    return new TextMatchExpression(content);
                }
            }
        }

        private static readonly Dictionary<string, Func<string, ContentMatchExpression>> Ctors =
            new Dictionary<string, Func<string, ContentMatchExpression>>
                {
                        { DatetimeBeginMatchExpression.ExpressionHeader, s => new DatetimeBeginMatchExpression(s) },
                        { DatetimeEndMatchExpression.ExpressionHeader, s => new DatetimeEndMatchExpression(s) },
                };
    }
}
