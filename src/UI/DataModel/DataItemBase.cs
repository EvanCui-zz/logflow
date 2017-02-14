using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public enum LogLevel
    {
        Critical = 0,
        Error = 1,
        Warning = 2,
        Info = 3,
        Verbose = 4,
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


        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Pid")]
        public int ProcessId { get; set; }
        public DisplayAttributes Display { get; set; }
    }
}
