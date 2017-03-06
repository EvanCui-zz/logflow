namespace LogFlow.DataModel
{
    using System;

    // Consider change it to struct and compact the integers.
    public class DataItemBase
    {
        [ColumnInfo(Name = "Id", Width = 50)]
        public int Id { get; set; }
        [ColumnInfo(Name = "Time", Width = 125)]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "Tid", Width = 50)]
        public int ThreadId { get; set; }

        [ColumnInfo(Name = "Level", Width = 50)]
        public LogLevels Level { get; set; }

        [ColumnInfo(Name = "Text", Computed = true, Width = 200)]

        public int TemplateId { get; set; } = -1;
        public object[] Parameters { get; set; }

        [ColumnInfo(Name = "Tag", Computed = true, Width = 60)]
        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Pid", Width = 60)]
        public int ProcessId { get; set; }
    }
}
