namespace LogFlow.DataModel
{
    public class ProgressItem
    {
        public ProgressItem(string actionName)
        {
            this.ActionName = actionName;
        }

        internal string ActionName { get; set; }

        public int Progress { get; set; }
        public string ProgressDescription => this.ActionName;
    }
}
