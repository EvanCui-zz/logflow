namespace LogFlow.DataModel
{
    public interface IFilter
    {
        string Name { get; }
        bool Match(DataItemStruct item, string template);
    }
}
