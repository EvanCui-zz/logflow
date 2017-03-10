namespace LogFilter
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics.Contracts;

    using LogFilter.Expressions;
    using LogFilter.Tokens;

    internal class ShuntingYardParser
    {
        private TokenInput input;

        private Stack<Token> operators;

        private Stack<Expression> operands;

        internal ShuntingYardParser(List<Token> tokens)
        {
            this.input = new TokenInput(tokens);
        }

        [Pure]
        private Token Next()
        {
            var peek = this.input.Peek();
            if (peek == null)
            {
                return new EndOfFileToken();
            }
            else return peek;
        }

        private void Consume()
        {
            this.input.Read();
        }

        private void Expect<T>() where T : Token
        {
            if (this.Next() is T)
            {
                this.Consume();
            }
            else
            {
                throw new ArgumentException($"Expected {typeof(T)} but get {this.Next().GetType()}");
            }
        }

        internal Expression Parse()
        {
            this.operators = new Stack<Token>();
            this.operands = new Stack<Expression>();
            this.operators.Push(new OpenParenthesisToken());
            this.E();
            this.Expect<EndOfFileToken>();
            return this.operands.Peek();
        }

        internal void E()
        {
            this.P();
            while (this.Next() is BinaryOperaterToken)
            {
                this.PushOperator(this.Next());
                this.Consume();
                this.P();
            }
            while (!(this.operators.Peek() is OpenParenthesisToken))
            {
                this.PopOperator();
            }
        }

        internal void P()
        {
            if (this.Next() is ContentToken)
            {
                this.operands.Push(MkLeaf(this.Next()));
                this.Consume();
            }
            else if (this.Next() is OpenParenthesisToken)
            {
                this.Consume();
                this.operators.Push(new OpenParenthesisToken());
                this.E();
                this.Expect<CloseParenthesisToken>();
                this.operands.Pop();
            }
            else if (this.Next() is UnaryOperatorToken)
            {
                this.PushOperator(this.Next());
                this.Consume();
                this.P();
            }
            else
            {
                throw new ArgumentException($"{this.Next().ToString()} is not a valid start of parameter");
            }
        }

        private void PushOperator(Token op)
        {
            // Here we use openParenthesisToken as starting token
            if (op is OpenParenthesisToken)
            {
                while (!(this.operators.Peek() is OpenParenthesisToken))
                {
                    this.PopOperator();
                }
            }
            this.operators.Push(op);
        }

        private void PopOperator()
        {
            if (this.operators.Peek() is BinaryOperaterToken)
            {
                Expression oprand2 = this.operands.Pop();
                Expression oprand1 = this.operands.Pop();
                this.operands.Push(MkNode(this.operators.Pop(), oprand1, oprand2));
            }
            else
            {
                this.operands.Push(MkNode(this.operators.Pop(), this.operands.Pop()));
            }
        }

        private static Expression MkNode(Token op, Expression oprand1, Expression oprand2)
        {
            if (op is LogicalAndToken)
            {
                return new LogicalAndExpression() { Lhs = oprand1, Rhs = oprand2 };
            }
            if (op is LogicalOrToken)
            {
                return new LogicalOrExpression() { Lhs = oprand1, Rhs = oprand2 };
            }
            throw new ArgumentException($"{op.ToString()} is not {nameof(BinaryOperaterToken)}");
        }

        private static Expression MkNode(Token op, Expression oprand)
        {
            if (op is LogicalNotToken)
            {
                return new LogicalNotExpression { Oprand = oprand };
            }
            throw new ArgumentException($"{op.ToString()} is not {nameof(UnaryOperatorToken)}");
        }

        private static Expression MkLeaf(Token token)
        {
            if (token is ContentToken)
            {
                return ContentMatchExpression.CreateContentMatchExpression(((ContentToken)token).Content);
            }
            throw new ArgumentException($"{token.ToString()} is not {nameof(ContentToken)}");
        }
    }
}
