using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class ProgressItem
    {
        public ProgressItem(string actionName)
        {
            this.ActionName = actionName;
        }

        private string ActionName { get; set; }

        public int Progress { get; set; }
        public string ProgressDescription { get { return $"{ActionName}"; } }
    }
}
