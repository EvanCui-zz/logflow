namespace LogFlow.DataModel
{
    public class CosmosDataItem : DataItemBase
    {
        [ColumnInfo(Computed = false, Name = "File", Width = 120)]
        public int FileIndex { get; set; }
    }
}
