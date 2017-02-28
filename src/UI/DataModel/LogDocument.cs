using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class RootView<T> : FilteredView<T> where T : DataItemBase
    {
        public RootView(string name, ILogSource<T> logSource, IFilter filter = null) : base(name)
        {
            this.Data = logSource;
        }
    }
}
