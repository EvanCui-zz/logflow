using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class FileFilter : IFilter
    {
        public string Name { get; private set; }
        private int FileIndex { get; set; }

        public FileFilter(string name, int fileIndex)
        {
            this.Name = name;
            this.FileIndex = fileIndex;
        }

        public bool Match<T>(T item, string template) where T : DataItemBase
        {
            // Here we do down cast, because IFilter cannot be contra-variant due to the co-variant constraint on ILogSource<T>.
            var cosmosDataItem = item as CosmosDataItem;
            return cosmosDataItem != null && cosmosDataItem.FileIndex == this.FileIndex;
        }
    }
}
