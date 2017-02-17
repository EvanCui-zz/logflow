using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class FilteredView<T> where T : DataItemBase
    {
        public FilteredView(Filter filter, FilteredView<T> parent, FilteredView<T> root)
        {
            this.Filter = filter;
            this.Name = this.Filter.Name;
            this.Parent = parent;
            this.Root = root;
            this.ItemIndexes = new List<int>();

            // todo: filter the item;
            if (this.Parent != null)
            {
                this.Parent.ItemAdded += (s, e) => { this.AddItem(e); };
            }
        }

        public int? FirstDisplayedScrollingRowIndex { get; set; }
        public bool IsInitialized { get; set; }

        public IDictionary<Color, Filter> Tags { get; set; } = new Dictionary<Color, Filter>();

        public IEnumerable<int> Find(Filter filter, int currentIndex, ResultWrapper<KeyValuePair<int, bool>> result, bool nextDirection)
        {
            if (!this.IsInitialized) yield break;

            yield return 0;
            this.Root.Suspend();
            yield return 5;

            int total = this.ItemIndexes?.Count ?? this.Items.Count;

            result.Result = new KeyValuePair<int, bool>(-1, true);
            for (int i = 1; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int realIndex = (currentIndex + (nextDirection ? i : -i) + total) % (total);

                int index = this.ItemIndexes?[realIndex] ?? realIndex;

                if (filter.Match<T>(this.Root.Items[index], this.Root.Templates[this.Root.Items[index].TemplateId]))
                {
                    result.Result = new KeyValuePair<int, bool>(realIndex, realIndex > currentIndex ^ nextDirection);
                    break;
                }
            }

            this.Root.Resume();
            yield return 100;
        }

        public IEnumerable<int> Count(Filter filter, ResultWrapper<int> result)
        {
            if (!this.IsInitialized) yield break;

            yield return 0;
            this.Root.Suspend();
            yield return 5;

            int total = this.ItemIndexes?.Count ?? this.Items.Count;

            int count = 0;

            for (int i = 0; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int index = this.ItemIndexes?[i] ?? i;

                if (filter.Match<T>(this.Root.Items[index], this.Root.Templates[this.Root.Items[index].TemplateId]))
                {
                    count++;
                }
            }

            result.Result = count;

            this.Root.Resume();
            yield return 100;
        }


        public IEnumerable<int> Initialize()
        {
            if (this.IsInitialized) yield break;

            if (this.Parent == null) { yield return 100; this.IsInitialized = true; yield break; }

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

            if (columnIndex == 5)
            {
                int index = this.ItemIndexes[rowIndex];
                T item = this.Root.Items[index];
                return this.Tags?.Where(kvp => kvp.Value.Match(item, this.Root.Templates[item.TemplateId])).Select(kvp => kvp.Key).ToList();
            }

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
