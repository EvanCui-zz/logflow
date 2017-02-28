using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataModel
{
    public interface IFilter
    {
        bool Match<T>(T item, string template) where T : DataItemBase;
    }
}
