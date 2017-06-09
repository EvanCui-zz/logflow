namespace LogFlow.DataModel.Algorithm
{
    using System.Collections.Generic;

    public class HashTablePool
    {
        private Dictionary<string, string> dict = new Dictionary<string, string>();

        public string Intern(string str)
        {
            return dict.TryGetValue(str, out string ans) ? ans : (dict[str] = str);
        }

        public void Intern(string[] strings)
        {
            for (int i = 0; i < strings.Length; i++) strings[i] = this.Intern(strings[i]);
        }
    }
}
