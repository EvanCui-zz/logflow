namespace LogFilter.Expressions
{
    using LogFlow.DataModel;

    internal abstract class Expression : IFilter
    {
        internal abstract string EvalToString();

        public string Name => this.EvalToString();

        public abstract bool Match<T>(T item, string template) where T : DataItemBase;
    }
}
