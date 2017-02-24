using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class DataSource<T> where T : DataItemBase
    {
        public DataSource()
        {
            this.PropertyInfos = typeof(T).GetProperties()
                .Where(f => f.IsDefined(typeof(ColumnInfoAttribute), true)).ToList();
            this.ColumnInfos = this.PropertyInfos.Select(p => p.GetCustomAttribute<ColumnInfoAttribute>(true)).ToList();
        }

        public IList<T> Items { get; set; } = new List<T>();

        public IList<string> Templates { get; } = new List<string>();

        public IList<PropertyInfo> PropertyInfos { get; set; }

        public IList<ColumnInfoAttribute> ColumnInfos { get; set; }

        public event EventHandler<int> ItemAdded;

        public void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        public void AddItem(T item)
        {
            if (this.IsSuspended)
            {
                throw new InvalidOperationException("The document is suspended.");
            }

            this.Items.Add(item);
            this.OnItemAdded(this.Items.Count - 1);
        }

        public object GetColumnValue(int rowIndex, int columnIndex)
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

        public void TestGenerateFakeData()
        {
            Random r = new Random();

            for (int i = 0; i < 1000; i++)
            {
                this.Templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, {0}", i));
            }

            for (int i = 0; i < 800000; i++)
            {
                var rand = r.Next(100);
                this.AddItem((T)new DataItemBase()
                {
                    Id = i,
                    ThreadId = i % 100,
                    Time = DateTime.UtcNow.AddSeconds(i),
                    TemplateId = r.Next(1000),
                    Parameters = new object[] { DateTime.UtcNow, i + 255 },
                    ProcessId = i / 100000,
                    Level = (LogLevel)(1 << (rand < 1 ? 0 : (rand < 5 ? 1 : (rand < 10 ? 2 : (rand < 55 ? 3 : 4))))),
                });
            }
        }

        private bool IsSuspended { get; set; }

        public void Suspend() { this.IsSuspended = true; }
        public void Resume() { this.IsSuspended = false; }
    }
}
