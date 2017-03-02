using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public interface ILogSource <out T> where T:DataItemBase
    {
        IReadOnlyList<T> Items { get; }
        IReadOnlyList<string> Templates { get; }
        IReadOnlyList<PropertyInfo> PropertyInfos { get; }
        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }
        // for performance, only pass int value
        event EventHandler<int> ItemAdded;
        object GetColumnValue(int rowIndex, int columnIndex);

        IEnumerable<int> Load(IFilter filter = null);
    }
}
