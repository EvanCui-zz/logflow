namespace LogFilter
{
    using System;

    public class ParsingException : Exception
    {
        public ParsingException(string message, int columnIndex)
            : base(message)
        {
            this.ColumnIndex = columnIndex;
        }

        public int ColumnIndex { get; }
    }
}
