namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Reflection;
    using System.Linq;

    // Consider change it to struct and compact the integers.
    public class DataItemBase
    {
        public readonly LogLevels[] LevelMapping = { LogLevels.None, LogLevels.Critical, LogLevels.Error, LogLevels.Warning, LogLevels.Info, LogLevels.Verbose, LogLevels.Detail };
        public readonly byte[] LevelReverseMapping =
        {
            0, 1, 2, 0, 3, 0, 0, 0,
            4, 0, 0, 0, 0, 0, 0, 0,
            5, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            6, 0, 0, 0, 0, 0, 0, 0
        };

        internal virtual bool Compress8(DateTime baseTime, out CompressedDataItem8 item)
        {
            item = new CompressedDataItem8();

            if ((this.ProcessId >> 8) > 0) return false;
            item.Pid = (byte)this.ProcessId;
            if ((this.ThreadId >> 8) > 0) return false;
            item.Tid = (byte)this.ThreadId;
            if ((this.ActivityIdIndex >> 8) > 0) return false;
            item.Aid = (byte)this.ActivityIdIndex;
            if ((this.FileIndex >> 5) > 0) return false;
            item.FileIndex = (byte)this.FileIndex;
            item.Level = LevelReverseMapping[(int)this.Level];

            int timeDiff = (int)(this.Time - baseTime).TotalSeconds;
            if ((timeDiff >> 16) > 0) return false;
            item.TimeOffsetSeconds = (ushort)timeDiff;

            if ((this.TemplateId >> 16) > 0) return false;
            item.TemplateId = (ushort)this.TemplateId;

            return true;
        }

        internal virtual void DeCompress8(CompressedDataItem8 item, DateTime baseTime)
        {
            this.ProcessId = item.Pid;
            this.ThreadId = item.Tid;
            this.ActivityIdIndex = item.Aid;
            this.FileIndex = item.FileIndex;
            this.Level = LevelMapping[item.Level];

            this.Time = baseTime.AddSeconds(item.TimeOffsetSeconds);
            this.TemplateId = item.TemplateId;
        }

        [ColumnInfo(Name = "Id", Width = 50)]
        public int Id { get; set; }

        [ColumnInfo(Name = "Time", Width = 125)]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "Tid", Width = 50)]
        public int ThreadId { get; set; }

        [ColumnInfo(Name = "Tag", Computed = true, Width = 60)]
        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Text", Computed = true, Width = 200)]
        public int TemplateId { get; set; } = -1;
        public string[] Parameters { get; set; }

        [ColumnInfo(Name = "ActId", Computed = false, Width = 120)]
        public int ActivityIdIndex { get; set; }

        [ColumnInfo(Name = "Pid", Width = 60)]
        public int ProcessId { get; set; }

        [ColumnInfo(Computed = false, Name = "File", Width = 120)]
        public int FileIndex { get; set; }

        [ColumnInfo(Name = "Level", Width = 50)]
        public LogLevels Level { get; set; }

        public static List<PropertyInfo> GetPropertyInfos<T>() where T : DataItemBase
        {
            var currentType = typeof(T);

            var propertyInfos = new List<PropertyInfo>(0);

            while (currentType != null)
            {
                propertyInfos = currentType.GetProperties()
                    .Where(f => f.IsDefined(typeof(ColumnInfoAttribute), true) && f.DeclaringType == currentType).Concat(propertyInfos).ToList();

                currentType = currentType.BaseType;
            }

            return propertyInfos;
        }

        public static List<ColumnInfoAttribute> GetColumnInfos(IEnumerable<PropertyInfo> propertyInfos) => propertyInfos.Select(p => p.GetCustomAttribute<ColumnInfoAttribute>(true)).ToList();
    }
}
