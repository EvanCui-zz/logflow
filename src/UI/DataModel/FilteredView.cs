using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class FilteredView<T> where T : DataItemBase
    {
        public Dictionary<string, FilteredView<T>> Children { get; } = new Dictionary<string, FilteredView<T>>();

        public FilteredView(string name, FilteredView<T> parent, FilteredView<T> root, List<int> itemIndexes)
        {
            this.Name = name;
            this.Parent = parent;
            this.Root = root;
            this.ItemIndexes = itemIndexes;

            // todo: filter the item;
            if (this.Parent != null)
            {
                this.Parent.ItemAdded += (s, e) => { this.AddItem(e); };
            }
        }

        public event EventHandler<int> ItemAdded;

        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        public string Name { get; private set; }

        public virtual int TotalCount { get { return this.ItemIndexes.Count; } }

        public FilteredView<T> Parent { get; private set; }
        public FilteredView<T> Root { get; private set; }

        public virtual object GetColumnValue(int rowIndex, int columnIndex)
        {
            return this.Root.GetColumnValue(this.ItemIndexes[rowIndex], columnIndex);
        }

        private void AddItem(int index)
        {
            this.ItemIndexes.Add(index);

            // passing the raw index directly to the child for performance.
            this.OnItemAdded(index);
        }
        
        private List<int> ItemIndexes { get; set; }
    }
}
