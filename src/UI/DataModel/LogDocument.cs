using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class LogDocument<T> : FilteredView<T> where T : DataItemBase
    {
        public LogDocument(string name) : base(name)
        {
            this.PropertyInfos = typeof(T).GetProperties()
                .Where(f => f.IsDefined(typeof(ColumnInfoAttribute), true)).ToList();
            this.ColumnInfos = this.PropertyInfos.Select(p => p.GetCustomAttribute<ColumnInfoAttribute>(true)).ToList();
            this.Root = this;
        }

        public override object GetColumnValue(int rowIndex, int columnIndex)
        {
            var dataItem = this.Items[rowIndex];

            PropertyInfo pi = this.PropertyInfos[columnIndex];

            ColumnInfoAttribute ci = this.ColumnInfos[columnIndex];

            if (!ci.Computed)
            {
                return pi.GetMethod.Invoke(dataItem, null);
            }
            else
            {
                return new ParametricString(
                    this.Templates[dataItem.TemplateId],
                    dataItem.Parameters);
            }
        }

        public override int TotalCount { get { return this.Items.Count; } }

        public override IList<T> Items { get; set; } = new List<T>();

        public override IList<string> Templates { get; } = new List<string>();

        public IList<PropertyInfo> PropertyInfos { get; set; }

        public IList<ColumnInfoAttribute> ColumnInfos
        {
            get; set;
        }

        private bool IsSuspended { get; set; }

        public override void Suspend() { this.IsSuspended = true; }
        public override void Resume() { this.IsSuspended = false; }

        public void AddItem(T item)
        {
            if (this.IsSuspended)
            {
                throw new InvalidOperationException("The document is suspended.");
            }

            this.Items.Add(item);
            this.OnItemAdded(this.Items.Count - 1);
        }

        public void TestGenerateFakeData()
        {
            Random r = new Random();

            for (int i = 0; i < 10000; i++)
            {
                this.Templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, {0}", i));
            }

            for (int i = 0; i < 100000; i++)
            {
                this.AddItem((T)new DataItemBase()
                {
                    Id = i,
                    ThreadId = i % 100,
                    Time = DateTime.UtcNow.AddSeconds(i),
                    TemplateId = r.Next(10000),
                    Parameters = new object[] { DateTime.UtcNow, i + 255 },
                    ProcessId = i / 100000,
                    Level = (LogLevel)r.Next((int)LogLevel.Verbose) + 1,
                });
            }
        }
    }
}
