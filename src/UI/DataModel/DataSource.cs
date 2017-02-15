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

        public virtual object GetColumnValue(int rowIndex, int columnIndex)
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
                return string.Intern(string.Format(
                    this.Templates[dataItem.TemplateId],
                    dataItem.Parameters.Select(p => $"{{{{{{{p}}}}}}}").ToArray()));
                //return dataItem.FormattedText ?? (dataItem.FormattedText = string.Intern(string.Format(this.Templates[dataItem.TemplateId], dataItem.Parameters)));
            }
        }

        public IList<T> Items { get; set; }

        public IList<string> Templates { get; set; }

        public IList<PropertyInfo> PropertyInfos { get; set; }

        public IList<ColumnInfoAttribute> ColumnInfos
        {
            get; set;
        }
    }
}
