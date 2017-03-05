using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public struct FullCosmosDataItem
    {
        public FullCosmosDataItem(CosmosDataItem item, string template) { this.Item = item; this.Template = template; }
        public CosmosDataItem Item { get; private set; }
        public string Template { get; private set; }
    }
}
