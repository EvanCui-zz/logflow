namespace LogFlow.DataModel
{
    public struct FullCosmosDataItem
    {
        public FullCosmosDataItem(CosmosDataItem item, string template) { this.Item = item; this.Template = template; }
        public CosmosDataItem Item { get; private set; }
        public string Template { get; private set; }
    }
}
