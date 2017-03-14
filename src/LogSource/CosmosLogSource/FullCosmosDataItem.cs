namespace LogFlow.DataModel
{
    public struct FullCosmosDataItem
    {
        public FullCosmosDataItem(CosmosDataItem item, string template, int percent)
        {
            this.Item = item;
            this.Template = template;
            this.Percent = percent;
        }

        public CosmosDataItem Item { get; }
        public string Template { get; }
        public int Percent { get; }
    }
}
