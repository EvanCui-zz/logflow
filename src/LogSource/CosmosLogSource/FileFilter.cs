namespace LogFlow.DataModel
{
    public class FileFilter : IFilter
    {
        public string Name { get; private set; }
        private int FileIndex { get; set; }

        public FileFilter(string name, int fileIndex)
        {
            this.Name = name;
            this.FileIndex = fileIndex;
        }

        public bool Match(DataItemStruct item, string template) 
        {
            // Here we do down cast, because IFilter cannot be contra-variant due to the co-variant constraint on ILogSource<T>.
            return item.FileIndex == this.FileIndex;
        }
    }
}
