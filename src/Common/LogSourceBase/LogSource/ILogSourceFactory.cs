namespace LogFlow.DataModel
{
    public interface ILogSourceFactory<out T> where T : DataItemBase
    {
        ILogSource<T> CreateInstance(string sourceInitializationData, LogSourceProperties properties);
    }
}
