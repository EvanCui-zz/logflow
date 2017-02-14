using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class DataItemBase
    {
        [ColumnInfo(Name = "Id", Width = 30, MinWidth = 10)]
        public long Id { get; set; }
        [ColumnInfo(Name = "Time", Width = 30, MinWidth = 10)]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "ThreadId", Width = 30, MinWidth = 10)]
        public int ThreadId { get; set; }

        [ColumnInfo(Name = "Text", Width = 300, MinWidth = 10, Computed = true)]
        public int TemplateId { get; set; }
        public object[] Parameters { get; set; }

        [ColumnInfo(Name = "ProcessId", Width = 30, MinWidth = 10)]
        public int ProcessId { get; set; }
        public DisplayAttributes Display { get; set; }
    }
}
