namespace LogFlow.Viewer.LogFilter.Expressions
{
    using LogFlow.DataModel;

    public abstract class Expression : IFilter
    {
        internal abstract string EvalToString(bool showOrder);

        public string Name => this.EvalToString(false);

        public abstract bool Match<T>(T item, string template) where T : DataItemBase;
    }
}
