namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Reflection;
    using System.Linq;

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
