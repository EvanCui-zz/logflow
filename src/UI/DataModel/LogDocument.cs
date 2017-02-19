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
            this.Data = new DataSource<T>();
            this.Data.ItemAdded += (s, e) =>
            {
                this.OnItemAdded(e);
            };
        }

        public void GenerateFakeData()
        {
            this.Data.TestGenerateFakeData();
        }
    }
}
