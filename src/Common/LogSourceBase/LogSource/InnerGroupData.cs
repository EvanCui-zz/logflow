using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class InnerGroupData
    {
        public List<int> InnerGroupIndexes { get; set; } = new List<int>();
        public int Percentage { get; set; }
    }
}
