using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace LogFlow.DataModel
{
    public struct DataItemStruct
    {
        //public DataItemStruct(DataItemStruct other)
        //{
        //    this.Id = other.Id;
        //    this.Time = other.Time;
        //    this.ThreadId = other.ThreadId;
        //    this.TemplateId = other.TemplateId;
        //    this.FormattedText = other.FormattedText;
        //    this.ActivityIdIndex = other.ActivityIdIndex;
        //    this.Parameters = other.Parameters;
        //    this.ProcessId = other.ProcessId;
        //    this.FileIndex = other.FileIndex;
        //    this.Level = other.Level;
        //}

        [ColumnInfo(Name = "Id", Width = 50)]
        public int Id { get; set; }

        [ColumnInfo(Name =  "Time", Width = 125)]
        public DateTime Time { get; set; }

        [ColumnInfo(Name = "Tid", Width = 50)]
        public int ThreadId { get; set; }

        [ColumnInfo(Name = "Tag", Computed = true, Width = 60)]
        public string FormattedText { get; set; }

        [ColumnInfo(Name = "Text", Computed = true, Width = 200)]
        public int TemplateId { get; set; }
        public string[] Parameters { get; set; }

        [ColumnInfo(Name = "ActId", Computed = false, Width = 120)]
        public int ActivityIdIndex { get; set; }

        [ColumnInfo(Name = "Pid", Width = 60)]
        public int ProcessId { get; set; }

        [ColumnInfo(Computed = false, Name = "File", Width = 120)]
        public int FileIndex { get; set; }

        [ColumnInfo(Name = "Level", Width = 50)]
        public LogLevels Level { get; set; }

        public static List<PropertyInfo> GetPropertyInfos()
        {
            var currentType = typeof(DataItemStruct);

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