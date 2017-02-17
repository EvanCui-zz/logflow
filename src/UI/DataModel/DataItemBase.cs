using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    [Flags]
    public enum LogLevel
    {
        None = 0x0,
        Critical = 0x1,
        Error = 0x2,
        Warning = 0x4,
        Info = 0x8,
        Verbose = 0x10,
    }

    public class DataItemBase
    {
        [ColumnInfo(Name = "Id")]
        public long Id { get; set; }
        [ColumnInfo(Name = "Time")]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "Tid")]
        public int ThreadId { get; set; }
        
        [ColumnInfo(Name = "Level")]
        public LogLevel Level { get; set; }

        [ColumnInfo(Name = "Text", Computed = true)]

        public int TemplateId { get; set; }
        public object[] Parameters { get; set; }

        [ColumnInfo(Name = "Tag", Computed = true)]
        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Pid")]
        public int ProcessId { get; set; }
        public DisplayAttributes Display { get; set; }
    }
}
