using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Reflection;

    public interface ILogSource<out T> where T : DataItemBase
    {
        string Name { get; }
        T this[int index] { get; }
        int Count { get; }

        bool CompressionEnabled { get; set; }
        bool AutoLoadingEnabled { get; set; }
        int Tier1Count { get; }
        int Tier2Count { get; }
        IReadOnlyList<string> Templates { get; }

        string GetHtml(IEnumerable<DataItemBase> items, bool withTitle);
        string GetText(IEnumerable<DataItemBase> items, bool withTitle);
        object GetColumnValue(DataItemBase item, int columnIndex);
        IReadOnlyList<PropertyInfo> PropertyInfos { get; }
        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }
        // for performance, only pass int value
        event EventHandler<int> ItemAdded;
      //  object GetColumnValue(int rowIndex, int columnIndex);

        IEnumerable<int> Peek(IFilter filter, int peekCount, CancellationToken token);

        IEnumerable<int> Load(IFilter filter, CancellationToken token);

        IReadOnlyList<IFilter> GroupFilters { get; }

        void InternStrings();
        //        IReadOnlyList<KeyValuePair<string, InnerGroupData>> GroupData { get; }
    }
}
