namespace LogFlow.DataModel
{
    public class ProgressItem
    {
        public ProgressItem(string actionName)
        {
            this.ActionName = actionName;
        }

        private string ActionName { get; }

        public int Progress { get; set; }
        public string ProgressDescription => this.ActionName;
    }
}
