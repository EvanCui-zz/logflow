using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public static class DataItemBaseExtension
    {
        public static string GetColumnText<T>(this T item, int column) where T : DataItemBase
        {
            switch(column)
            {
                case 1:
                    return item.Id.ToString();
                case 2:
                    return item.Time.ToString();
                case 3:
                    return item.ThreadId.ToString();
                case 4:
                    return item.Level.ToString();
                case 7:
                    return item.ProcessId.ToString();
                default:
                    return null;
            }
        }
    }
}
