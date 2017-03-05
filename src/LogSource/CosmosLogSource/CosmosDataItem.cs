using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class CosmosDataItem : DataItemBase
    {
        [ColumnInfo(Computed = false, Name = "File", Width = 120)]
        public int FileIndex { get; set; }
    }
}
