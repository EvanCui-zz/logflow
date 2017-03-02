using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
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
