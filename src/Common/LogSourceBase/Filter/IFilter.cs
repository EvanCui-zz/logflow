namespace LogFlow.DataModel
{
    public interface IFilter
    {
        string Name { get; }
        bool Match<T>(T item, string template) where T : DataItemBase;
    }
}
