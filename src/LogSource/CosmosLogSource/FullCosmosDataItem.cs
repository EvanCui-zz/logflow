namespace LogFlow.DataModel
{
    public struct FullDataItemStruct
    {
        public FullDataItemStruct(DataItemStruct item, string template) { this.Item = item; this.Template = template; }
        public DataItemStruct Item { get; private set; }
        public string Template { get; private set; }
    }
}
