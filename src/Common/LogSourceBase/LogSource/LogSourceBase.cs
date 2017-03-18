using System.Threading;
using LogFlow.DataModel.Algorithm;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Reflection;

    public abstract class LogSourceBase<T> : ILogSource<T> where T : DataItemBase
    {
        protected LogSourceBase()
        {
            this.propertyInfos = DataItemBase.GetPropertyInfos<T>();

            this.columnInfos = DataItemBase.GetColumnInfos(propertyInfos);
        }

        public abstract string Name { get; }

        public bool CompressionEnabled { get; set; }
        public bool AutoLoadingEnabled { get; set; }
        public virtual int Count => this.InternalItems.Count;
        public virtual int Tier1Count { get; } = 0;
        public virtual int Tier2Count { get; } = 0;

        public virtual T this[int index] => this.InternalItems[index];

        protected List<T> InternalItems = new List<T>();
        protected List<string[]> Parameters = new List<string[]>();

        protected IdentifierCache<string> files = new IdentifierCache<string>();

        public IReadOnlyList<string> Templates => this.templates;
        private readonly IdentifierCache<string> templates = new IdentifierCache<string>();

        public IReadOnlyList<PropertyInfo> PropertyInfos => this.propertyInfos;
        private readonly List<PropertyInfo> propertyInfos;

        public IReadOnlyList<ColumnInfoAttribute> ColumnInfos => this.columnInfos;
        private readonly List<ColumnInfoAttribute> columnInfos;

        public IReadOnlyList<IFilter> GroupFilters => InnerGroupFilters;
        protected List<IFilter> InnerGroupFilters = null;

        public virtual object GetColumnValue(DataItemBase item, int columnIndex)
        {
            ColumnInfoAttribute ci = this.ColumnInfos[columnIndex];

            if (string.Equals(ci.Name, "Text", StringComparison.Ordinal))
            {
                return new ParametricString(
                    this.Templates[item.TemplateId],
                    item.Parameters);
            }
            else if (string.Equals(ci.Name, "Time", StringComparison.Ordinal))
            {
                return item.Time.ToString("s");
            }
            else if (string.Equals(ci.Name, "File", StringComparison.Ordinal))
            {
                return this.files[item.FileIndex];
            }
            else
            {
                PropertyInfo pi = this.PropertyInfos[columnIndex];
                return pi.GetMethod.Invoke(item, null);
            }
        }

        // for performance, only pass int value
        public event EventHandler<int> ItemAdded;
        protected void OnItemAdded(int index)
        {
            this.ItemAdded?.Invoke(this, index);
        }

        protected virtual void AddItem(FullDataItem<T> item)
        {
            this.InternalItems.Add(item.Item);
            item.Item.Id = this.InternalItems.Count - 1;
            this.OnItemAdded(item.Item.Id);
        }

        protected int AddTemplate(string template)
        {
            return this.templates.Put(template);
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
        private int lastParameterInternIndex;
        public virtual void InternStrings()
        {
            while (this.lastInternIndex < this.InternalItems.Count)
            {
                this.InternalItems[this.lastInternIndex].Parameters = this.InternalItems[this.lastInternIndex].Parameters.Select(LocalStringPool.Intern).ToArray();
                this.lastInternIndex++;
            }

            while (this.lastParameterInternIndex < this.Parameters.Count)
            {
                this.Parameters[this.lastParameterInternIndex] = this.Parameters[this.lastParameterInternIndex].Select(LocalStringPool.Intern).ToArray();
                this.lastParameterInternIndex++;
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
