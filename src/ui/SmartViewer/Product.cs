using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LogFlow.Viewer
{
    public static class Product
    {
        public static string GetTitle()
        {
            return $"{Application.ProductName} - {Application.ProductVersion} - beta";
        }
    }
}
