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

    // Consider change it to struct and compact the integers.
    public class DataItemBase
    {
        [ColumnInfo(Name = "Id", Width = 50)]
        public long Id { get; set; }
        [ColumnInfo(Name = "Time", Width = 125)]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "Tid", Width = 50)]
        public int ThreadId { get; set; }

        [ColumnInfo(Name = "Level", Width = 50)]
        public LogLevel Level { get; set; }

        [ColumnInfo(Name = "Text", Computed = true, Width = 200)]

        public int TemplateId { get; set; }
        public object[] Parameters { get; set; }

        [ColumnInfo(Name = "Tag", Computed = true, Width = 60)]
        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Pid", Width = 60)]
        public int ProcessId { get; set; }
    }
}
