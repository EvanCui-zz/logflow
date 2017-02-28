using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public abstract class LogSourceBase<T> : ILogSource<T> where T : DataItemBase
    {
        public LogSourceBase()
        {
            this.propertyInfos = typeof(T).GetProperties()
                .Where(f => f.IsDefined(typeof(ColumnInfoAttribute), true)).ToList();
            this.columnInfos = this.PropertyInfos.Select(p => p.GetCustomAttribute<ColumnInfoAttribute>(true)).ToList();
        }
        public IReadOnlyList<T> Items
        {
            get { return this.items; }
        }

        protected List<T> items = new List<T>();

        public IReadOnlyList<string> Templates { get { return this.templates; } }
        protected List<string> templates = new List<string>();

        public IReadOnlyList<PropertyInfo> PropertyInfos { get { return this.propertyInfos; } }
        private List<PropertyInfo> propertyInfos;

        public IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get { return this.columnInfos; } }
        private List<ColumnInfoAttribute> columnInfos;

        // for performance, only pass int value
        public event EventHandler<int> ItemAdded;
        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        protected void AddItem(T item)
        {
            this.items.Add(item);
            this.OnItemAdded(this.Items.Count - 1);
        }

        public virtual object GetColumnValue(int rowIndex, int columnIndex)
        {
            var dataItem = this.Items[rowIndex];

            PropertyInfo pi = this.PropertyInfos[columnIndex];

            ColumnInfoAttribute ci = this.ColumnInfos[columnIndex];

            if (!ci.Computed)
            {
                return pi.GetMethod.Invoke(dataItem, null);
            }
            else if (ci.Name == "Text")
            {
                return new ParametricString(
                    this.Templates[dataItem.TemplateId],
                    dataItem.Parameters);
            }
            else
            {
                return null;
            }
        }

        public abstract IEnumerable<int> Load(IFilter filter);
    }
}
