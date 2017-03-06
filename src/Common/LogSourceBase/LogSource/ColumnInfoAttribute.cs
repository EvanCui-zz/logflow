namespace LogFlow.DataModel
{
    using System;

    public class ColumnInfoAttribute : Attribute
    {
        public string Name
        {
            get; set;
        }

        public bool Computed { get; set; }

        public int Width { get; set; }
    }
}
