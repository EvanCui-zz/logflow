using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Reflection;

    public interface ILogSource<out T> where T : DataItemBase
    {
        string Name { get; }
        IReadOnlyList<T> Items { get; }
        IReadOnlyList<string> Templates { get; }
        IReadOnlyList<PropertyInfo> PropertyInfos { get; }
        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }
        // for performance, only pass int value
        event EventHandler<int> ItemAdded;
        object GetColumnValue(int rowIndex, int columnIndex);

        IEnumerable<int> Load(IFilter filter, bool stopAtFirst, CancellationToken token);

        IReadOnlyList<IFilter> GroupFilters { get; }
        //        IReadOnlyList<KeyValuePair<string, InnerGroupData>> GroupData { get; }
    }
}
