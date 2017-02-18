﻿using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public class FilteredView<T> : IFilteredView<T> where T : DataItemBase
    {
        #region Find, Count, Filter, Tag features.

        /// <summary>
        /// Tag or replace the color's filter.
        /// </summary>
        /// <param name="tagColor">the tag color</param>
        /// <param name="filter">the filter</param>
        public void Tag(Color tagColor, Filter filter)
        {
            this.Tags[tagColor] = filter;
        }

        public void UnTag(Color tagColor)
        {
            this.Tags.Remove(tagColor);
        }

        /// <summary>
        /// Find the next occurrence. Yielding progress from 0 to 100, if -1 is yielded, it means no result till the end of the current direction.
        /// Continue iterating the progress will search from the other end. If -2 is yielded, it means no result for all items.
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="currentIndex"></param>
        /// <param name="direction"></param>
        /// <param name="result"></param>
        /// <returns></returns>
        public IEnumerable<int> Find(Filter filter, int currentIndex, bool direction, ResultWrapper<int> result)
        {
            result.Result = -2;
            if (!this.IsInitialized) yield break;

            yield return 0;
            this.Data.Suspend();
            yield return 5;

            int total = this.ItemIndexes?.Count ?? this.Data.Items.Count;

            bool loopedBack = false;
            for (int i = 1; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int logicalIndex = (currentIndex + (direction ? i : -i) + total) % (total);
                if (!loopedBack && (direction ^ currentIndex < logicalIndex))
                {
                    loopedBack = true;
                    yield return -1;
                }

                int index = this.GetPhysicalIndex(logicalIndex);

                if (filter.Match<T>(this.Data.Items[index], this.Data.Templates[this.Data.Items[index].TemplateId]))
                {
                    result.Result = logicalIndex;
                    break;
                }
            }

            if (result.Result == -2)
            {
                yield return -2;
            }

            this.Data.Resume();
            yield return 100;
        }

        public IEnumerable<int> Count(Filter filter, ResultWrapper<int> result)
        {
            result.Result = 0;
            if (!this.IsInitialized) yield break;

            yield return 0;
            this.Data.Suspend();
            yield return 5;

            int total = this.TotalCount;
            int count = 0;

            for (int i = 0; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int index = this.GetPhysicalIndex(i);

                if (filter.Match<T>(this.Data.Items[index], this.Data.Templates[this.Data.Items[index].TemplateId]))
                {
                    count++;
                }
            }

            result.Result = count;

            this.Data.Resume();
            yield return 100;
        }

        public IEnumerable<int> Initialize()
        {
            if (this.IsInitialized) yield break;

            if (this.Parent == null) { yield return 100; this.IsInitialized = true; yield break; }

            yield return 0;
            this.Data.Suspend();
            yield return 5;

            int total = this.Parent.TotalCount;

            for (int i = 0; i < total; i++)
            {
                int progress = (i * 100) / total;
                if (progress % 5 == 0)
                {
                    yield return 5 + (progress * 90) / 100;
                }

                int index = this.Parent.GetPhysicalIndex(i);

                if (this.Filter.Match<T>(this.Data.Items[index], this.Data.Templates[this.Data.Items[index].TemplateId]))
                {
                    this.ItemIndexes.Add(index);
                }
            }

            this.Data.Resume();
            this.IsInitialized = true;
            yield return 100;
        }

        public bool IsInitialized { get; private set; }

        #endregion

        #region Events

        public event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        public FilteredView<T> CreateChild(Filter filter)
        {
            return new FilteredView<T>(filter, this, this.Data);
        }

        #endregion

        #region Display

        public int? FirstDisplayedScrollingRowIndex { get; set; }

        public string Name { get; private set; }

        public int TotalCount { get { return this.ItemIndexes?.Count ?? this.Data.Items.Count; } }

        public object GetColumnValue(int rowIndex, int columnIndex)
        {
            if (rowIndex >= this.TotalCount) return null;
            int index = this.GetPhysicalIndex(rowIndex);

            // only process the tag column
            if (string.Equals(this.Data.ColumnInfos[columnIndex].Name, "Tag"))
            {
                T item = this.Data.Items[index];
                return this.Tags?.Where(kvp => kvp.Value.Match(item, this.Data.Templates[item.TemplateId])).Select(kvp => kvp.Key).ToList();
            }

            return this.Data.GetColumnValue(index, columnIndex);
        }

        public int GetPhysicalIndex(int logicalIndex)
        {
            return this.ItemIndexes?[logicalIndex] ?? logicalIndex;
        }

        public IList<ColumnInfoAttribute> ColumnInfos { get { return this.Data.ColumnInfos; } }

        #endregion

        #region Constructors

        internal FilteredView(Filter filter, FilteredView<T> parent, DataSource<T> data) : this(filter.Name)
        {
            this.Filter = filter;
            this.Parent = parent;
            this.Data = data;
            this.ItemIndexes = new List<int>();

            // todo: filter the item;
            if (this.Parent != null)
            {
                this.Parent.ItemAdded += (s, e) =>
                {
                    if (filter.Match(this.Data.Items[e], this.Data.Templates[this.Data.Items[e].TemplateId]))
                    {
                        this.AddItem(e);
                    }
                };
            }
        }

        protected FilteredView(string name)
        {
            this.Name = name;
        }

        #endregion

        #region Properties and fields

        private IDictionary<Color, Filter> Tags { get; set; } = new Dictionary<Color, Filter>();

        private Filter Filter { get; set; }

        private FilteredView<T> Parent { get; set; }

        internal DataSource<T> Data { get; set; }

        private List<int> ItemIndexes { get; set; }

        #endregion

        #region Private methods

        private void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        private void AddItem(int index)
        {
            this.ItemIndexes.Add(index);

            // passing the raw index directly to the child for performance.
            this.OnItemAdded(index);
        }

        #endregion
    }
}