namespace LogFlow.DataModel
{
    using System;

    public class FullCosmosDataItem : FullDataItem<CosmosDataItem>
    {
        public FullCosmosDataItem(CosmosDataItem item, string template, Guid activityId, int percent) : base(item, template, activityId, percent)
        {
        }
    }
}
