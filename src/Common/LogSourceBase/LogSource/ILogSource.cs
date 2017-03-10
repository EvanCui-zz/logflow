using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Reflection;

    public interface ILogSource
    {
        string Name { get; }
        IReadOnlyList<DataItemStruct> Items { get; }
        IReadOnlyList<string> Templates { get; }
        IReadOnlyList<PropertyInfo> PropertyInfos { get; }
        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }
        // for performance, only pass int value
        event EventHandler<int> ItemAdded;
        object GetColumnValue(int rowIndex, int columnIndex);

        IEnumerable<int> Peek(IFilter filter, int peekCount, CancellationToken token);

        IEnumerable<int> Load(IFilter filter, CancellationToken token);

        IReadOnlyList<IFilter> GroupFilters { get; }

        void InternStrings();
        //        IReadOnlyList<KeyValuePair<string, InnerGroupData>> GroupData { get; }
    }
}
