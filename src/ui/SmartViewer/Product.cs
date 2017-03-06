namespace LogFlow.Viewer
{
    using System.Windows.Forms;

    public static class Product
    {
        public static string GetTitle()
        {
            return $"{Application.ProductName} - {Application.ProductVersion} - beta";
        }
    }
}
