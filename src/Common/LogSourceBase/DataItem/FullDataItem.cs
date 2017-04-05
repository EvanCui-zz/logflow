namespace LogFlow.DataModel
{
    using System;

    public class FullDataItem<T> where T: DataItemBase
    {
        public FullDataItem(T item, string template, Guid activityId, int percent)
        {
            this.Item = item;
            this.Template = template;
            this.ActivityId = activityId;
            this.Percent = percent;
        }

        public T Item { get; }
        public string Template { get; }
        public Guid ActivityId { get; }
        public int Percent { get; }
    }
}
