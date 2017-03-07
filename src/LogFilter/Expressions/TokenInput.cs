namespace LogFilter.Expressions
{
    using System.Collections.Generic;

    using LogFilter.Tokens;

    internal class TokenInput
    {
        internal List<Token> input;

        private int idx = -1;

        private int length => this.input.Count;

        internal TokenInput(List<Token> tokens)
        {
            this.input = tokens;
        }

        internal Token Peek()
        {
            if (this.idx < this.length - 1)
            {
                return this.input[this.idx + 1];
            }
            else
            {
                return null;
            }
        }

        internal void Unread()
        {
            this.PreviousIdx();
        }

        internal Token Read()
        {
            this.NextIdx();
            if (this.idx < this.length)
            {
                return this.input[this.idx];
            }
            else
            {
                return null;
            }
        }

        private void NextIdx() => this.idx++;

        private void PreviousIdx() => this.idx--;
    }
}