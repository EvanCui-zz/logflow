using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LogFlow.DataModel;

namespace LowFlow.DataModel
{
    public class CosmosDataItem : DataItemBase
    {
        [ColumnInfo(Computed = false, Name = "File", Width = 50)]
        public int FileIndex { get; set; }
    }
}
