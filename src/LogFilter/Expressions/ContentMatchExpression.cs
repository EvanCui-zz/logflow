namespace LogFilter.Expressions
{
    using System;
    using System.Collections.Generic;

    using LogFilter.Tokens;

    internal abstract class ContentMatchExpression : UnaryExpression
    {
        internal static ContentMatchExpression CreateContentMatchExpression(ContentToken token)
        {
            string header = token.Content.Substring(0, token.Content.IndexOf(':') + 1);
            if (string.IsNullOrWhiteSpace(header))
            {
                return new TextMatchExpression(token);
            }
            else
            {
                Func<ContentToken, ContentMatchExpression> ctor;
                if (Ctors.TryGetValue(header, out ctor))
                {
                    return ctor(token);
                }
                else
                {
                    return new TextMatchExpression(token);
                }
            }
        }

        private static readonly Dictionary<string, Func<ContentToken, ContentMatchExpression>> Ctors =
            new Dictionary<string, Func<ContentToken, ContentMatchExpression>>
                {
                        { DatetimeBeginMatchExpression.ExpressionHeader, c => new DatetimeBeginMatchExpression(c) },
                        { DatetimeEndMatchExpression.ExpressionHeader, c => new DatetimeEndMatchExpression(c) },
                        { ProcessIdMatchExpression.ExpressionHeader, c => new ProcessIdMatchExpression(c) },
                        { ThreadIdMatchExpression.ExpressionHeader, c => new ThreadIdMatchExpression(c) },
                        { LogLevelMatchExpression.ExpressionHeader, c => new LogLevelMatchExpression(c) },
                };
    }
}
