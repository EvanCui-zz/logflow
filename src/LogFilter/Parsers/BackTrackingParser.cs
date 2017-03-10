namespace LogFilter
{
    using System;
    using System.Collections.Generic;

    using LogFilter.Expressions;
    using LogFilter.Tokens;

    internal class Parser
    {
        private int parenthesisStackHeight = 0;

        private Expression ASTRoot = null;

        private TokenInput input;

        internal Parser(List<Token> tokens)
        {
            this.input = new TokenInput(tokens);
        }

        internal static Expression Parse(List<Token> tokens)
        {
            Parser astc = new Parser(tokens);
            return astc.ParseEx();
        }

        internal Expression ParseEx()
        {
            Expression res;
            res = this.ParseLogicNot();
            if (res != null)
            {
                return res;
            }

            res = this.ParseParenthesised();
            if (res != null)
            {
                return res;
            }

            res = this.ParseContent();
            if (res != null)
            {
                return res;
            }

            return null;
        }

        internal Expression ParseExPrime(Expression ex)
        {
            Expression res;
            res = this.ParseLogicalAnd(ex);
            if (res != null)
            {
                return res;
            }

            res = this.ParseLogicalOr(ex);
            if (res != null)
            {
                return res;
            }

            res = this.ParseImplicitLogicalAnd(ex);
            if (res != null)
            {
                return res;
            }

            res = this.ParseEpsilon(ex);
            if (res != null)
            {
                return res;
            }

            throw new ArgumentException("Failed to parse Ex'");
        }

        internal Expression ParseLogicNot() // E -> !E E'
        {
            if (this.input.Read() is LogicalNotToken)
            {
                Expression ex = this.ParseEx();
                if (ex != null)
                {
                    Expression res;
                    if (ex is UnaryExpression)
                    {
                        res = new LogicalNotExpression() { Oprand = ex };
                        return this.ParseExPrime(res);
                    }
                    else
                    {
                        // BinaryExpression
                        BinaryExpression be = (BinaryExpression)ex;
                        res = new LogicalNotExpression() { Oprand = be.Lhs };
                        be.Lhs = res;
                        return be;
                    }
                }
                else
                {
                    throw new ArgumentException("Failed parse LogicNot");
                }
            }
            else
            {
                this.input.Unread();
                return null;
            }
        }

        internal Expression ParseParenthesised() // E -> (E) E'
        {
            if (this.input.Read() is OpenParenthesisToken)
            {
                var internalTokens = new List<Token>();
                int parenthesisNum = 1;
                while (true)
                {
                    var t = this.input.Read();
                    if (t == null)
                    {
                        throw new ArgumentException("Parenthesis not closed.");
                    }
                    else if (t is CloseParenthesisToken)
                    {
                        if (--parenthesisNum <= 0)
                        {
                            break;
                        }
                    }
                    else if (t is OpenParenthesisToken)
                    {
                        ++parenthesisNum;
                    }

                    internalTokens.Add(t);
                }

                Expression ex = Parse(internalTokens);
                if (ex != null)
                {
                    Expression res = new ParenthesisedExpression() { Oprand = ex };
                    return this.ParseExPrime(res);
                }
                else
                {
                    throw new ArgumentException("Failed parse Parenthesised Expression");
                }
            }
            else
            {
                this.input.Unread();
                return null;
            }
        }

        internal Expression ParseContent() // E -> C E'
        {
            if (this.input.Peek() is ContentToken)
            {
                ContentToken ct = (ContentToken)this.input.Read();
                Expression res = ContentMatchExpression.CreateContentMatchExpression(ct.Content);
                return this.ParseExPrime(res);
            }
            else
            {
                return null;
            }
        }

        internal Expression ParseLogicalAnd(Expression lhs) // E' -> && E E'
        {
            if (this.input.Read() is LogicalAndToken)
            {
                Expression rhs = this.ParseEx();
                if (rhs != null)
                {
                    Expression res = new LogicalAndExpression() { Lhs = lhs, Rhs = rhs };
                    return this.ParseExPrime(res);
                }
                else
                {
                    throw new ArgumentException("Failed to parsed LogicalAnd");
                }
            }
            else
            {
                this.input.Unread();
                return null;
            }
        }

        internal Expression ParseLogicalOr(Expression lhs) // E' -> || E E'
        {
            if (this.input.Read() is LogicalOrToken)
            {
                Expression rhs = this.ParseEx();
                if (rhs != null)
                {
                    Expression res = new LogicalOrExpression() { Lhs = lhs, Rhs = rhs };
                    return this.ParseExPrime(res);
                }
                else
                {
                    throw new ArgumentException("Failed to parsed LogicalAnd");
                }
            }
            else
            {
                this.input.Unread();
                return null;
            }
        }

        internal Expression ParseImplicitLogicalAnd(Expression lhs) // E' -> E E'
        {
            Expression rhs = this.ParseEx();
            if (rhs != null)
            {
                Expression res = new LogicalAndExpression() { Lhs = lhs, Rhs = rhs };
                return this.ParseExPrime(res);
            }
            else
            {
                return null;
            }
        }

        internal Expression ParseEpsilon(Expression ex) // E' -> epsilon
        {
            if (this.input.Read() == null)
            {
                return ex;
            }

            return null;
        }
    }
}
