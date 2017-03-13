namespace LogFilter.Expressions
{
    using System;
    using System.Collections.Generic;

    using LogFilter.Tokens;

    internal abstract class ContentMatchExpression : Expression
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

        internal override string EvalToString(bool showOrder)
        {
            string res = this.EvalToStringAcc();
            if (showOrder)
            {
                return $"({res})";
            }
            else
            {
                return res;
            }
        }

        protected abstract string EvalToStringAcc();

        private static readonly Dictionary<string, Func<ContentToken, ContentMatchExpression>> Ctors =
            new Dictionary<string, Func<ContentToken, ContentMatchExpression>>
                {
                        { DatetimeBeginMatchExpression.ExpressionHeader, c => new DatetimeBeginMatchExpression(c) },
                        { DatetimeEndMatchExpression.ExpressionHeader, c => new DatetimeEndMatchExpression(c) },
                        { ProcessIdMatchExpression.ExpressionHeader, c => new ProcessIdMatchExpression(c) },
                        { ThreadIdMatchExpression.ExpressionHeader, c => new ThreadIdMatchExpression(c) },
                        { LogLevelMatchExpression.ExpressionHeader, c => new LogLevelMatchExpression(c) },
                        { RegexMatchExpression.ExpressionHeader, c => new RegexMatchExpression(c) },
                };

        protected string RestoreEscaped(string str)
        {
            if (str.StartsWith("\"") && str.EndsWith("\""))
            {
                return str.Substring(1, str.Length - 2).Replace("\"\"", "\"");
            }
            else
            {
                return str;
            }
        }
    }
}
