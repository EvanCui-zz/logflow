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
            this.Data = this;
        }



    }
}
