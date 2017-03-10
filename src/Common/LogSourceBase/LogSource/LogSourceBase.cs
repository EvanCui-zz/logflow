using System.Threading;
using LogFlow.DataModel.Algorithm;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;

    public abstract class LogSourceBase : ILogSource
    {
        protected LogSourceBase()
        {
            this.propertyInfos = DataItemStruct.GetPropertyInfos();

            this.columnInfos = DataItemStruct.GetColumnInfos(propertyInfos);
        }

        public abstract string Name { get; }

        public IReadOnlyList<DataItemStruct> Items => this.InternalItems;

        protected List<DataItemStruct> InternalItems = new List<DataItemStruct>();

        public IReadOnlyList<string> Templates => this.templates;
        private readonly List<string> templates = new List<string>();
        private readonly Dictionary<string, int> templatesIndex = new Dictionary<string, int>();

        public IReadOnlyList<PropertyInfo> PropertyInfos => this.propertyInfos;
        private readonly List<PropertyInfo> propertyInfos;

        public IReadOnlyList<ColumnInfoAttribute> ColumnInfos => this.columnInfos;
        private readonly List<ColumnInfoAttribute> columnInfos;

        public IReadOnlyList<IFilter> GroupFilters => InnerGroupFilters;
        protected List<IFilter> InnerGroupFilters = null;

        //public IReadOnlyList<KeyValuePair<string, InnerGroupData>> GroupData => this.InnerGroupData;
        //protected List<KeyValuePair<string, InnerGroupData>> InnerGroupData = null;

        // for performance, only pass int value
        public event EventHandler<int> ItemAdded;
        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        protected void AddItem(DataItemStruct item)
        {
            this.InternalItems.Add(item);
            item.Id = this.InternalItems.Count - 1;
            this.OnItemAdded(item.Id);
        }

        protected int AddTemplate(string template)
        {
            //template = LocalStringPool.Intern(template);

            int index;
            if (this.templatesIndex.TryGetValue(template, out index))
            {
                return index;
            }
            else
            {
                this.templates.Add(template);
                return this.templatesIndex[template] = this.templates.Count - 1;
            }
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
            else if (ci.Name == "Text")
            {
                return new ParametricString(
                    this.Templates[dataItem.TemplateId],
                    dataItem.Parameters);
            }
            else
            {
                return null;
            }
        }

        private bool firstBatchLoaded;
        protected int CurrentId = 0;

        public virtual IEnumerable<int> Peek(IFilter filter, int peekCount, CancellationToken token) { yield break; }

        public virtual IEnumerable<int> Load(IFilter filter, CancellationToken token)
        {
            if (firstBatchLoaded) return this.LoadIncremental(filter, token);

            firstBatchLoaded = true;
            return this.LoadFirst(filter, token);
        }

        protected virtual IEnumerable<int> LoadFirst(IFilter filter, CancellationToken token) { yield break; }
        protected virtual IEnumerable<int> LoadIncremental(IFilter filter, CancellationToken token) { yield break; }

        private int lastInternIndex;
        private int lastTemplateInternIndex;
        public void InternStrings()
        {
            while (this.lastInternIndex < this.Items.Count)
            {
                var di = this.InternalItems[this.lastInternIndex];
                di.Parameters = this.Items[this.lastInternIndex].Parameters.Select(LocalStringPool.Intern).ToArray();
                this.InternalItems[this.lastInternIndex] = di;
                this.lastInternIndex++;
            }

            while (this.lastTemplateInternIndex < this.templates.Count)
            {
                this.templates[this.lastTemplateInternIndex] =
                    LocalStringPool.Intern(this.templates[this.lastTemplateInternIndex]);
                this.lastTemplateInternIndex++;
            }
        }
    }
}
