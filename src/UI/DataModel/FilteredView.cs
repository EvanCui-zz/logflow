using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class FilteredView<T> where T : DataItemBase
    {
        public FilteredView(Filter filter, FilteredView<T> parent, FilteredView<T> root, List<int> itemIndexes)
        {
            this.Filter = filter;
            this.Name = this.Filter.Name;
            this.Parent = parent;
            this.Root = root;
            this.ItemIndexes = itemIndexes;

            // todo: filter the item;
            if (this.Parent != null)
            {
                this.Parent.ItemAdded += (s, e) => { this.AddItem(e); };
            }
        }

        public FilteredView(Filter filter, FilteredView<T> parent, FilteredView<T> root)
        {
            this.Filter = filter;
            this.Name = this.Filter.Name;
            this.Parent = parent;
            this.Root = root;
            this.ItemIndexes = new List<int>();
            
            if (this.Parent != null)
            {
                this.Parent.ItemAdded += (s, e) => { this.AddItem(e); };
            }
        }

        public bool IsInitialized { get; set; }

        public IEnumerable<int> Initialize()
        {
            if (this.IsInitialized) yield break;

            if (this.Parent == null) { yield return 100; yield break; }

            yield return 0;
            this.Root.Suspend();
            yield return 5;

            int total = this.Parent.ItemIndexes?.Count ?? this.Parent.Items.Count;

            for (int i = 0; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int index = this.Parent.ItemIndexes?[i] ?? i;

                if (this.Filter.Match<T>(this.Root.Items[index], this.Root.Templates[this.Root.Items[index].TemplateId]))
                {
                    this.ItemIndexes.Add(index);
                }
            }

            this.Root.Resume();
            this.IsInitialized = true;
            yield return 100;
        }

        protected FilteredView(string name)
        {
            this.Name = name;
        }

        public FilteredView<T> CreateChild(Filter filter)
        {
            return new FilteredView<T>(filter, this, this.Root);
        }

        public virtual void Suspend() { }

        public virtual void Resume() { }

        public event EventHandler<int> ItemAdded;

        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        private Filter Filter { get; set; }
        public string Name { get; set; }

        public virtual int TotalCount { get { return this.ItemIndexes.Count; } }

        public FilteredView<T> Parent { get; private set; }
        public FilteredView<T> Root { get; protected set; }

        public virtual object GetColumnValue(int rowIndex, int columnIndex)
        {
            if (rowIndex >= this.ItemIndexes.Count) return null;
            return this.Root.GetColumnValue(this.ItemIndexes[rowIndex], columnIndex);
        }

        private void AddItem(int index)
        {
            this.ItemIndexes.Add(index);

            // passing the raw index directly to the child for performance.
            this.OnItemAdded(index);
        }

        private List<int> ItemIndexes { get; set; }
        public virtual IList<T> Items { get; set; } = null;
        public virtual IList<string> Templates { get { return this.Root.Templates; } }
    }
}
