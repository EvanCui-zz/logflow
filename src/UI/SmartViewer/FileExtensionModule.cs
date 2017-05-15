namespace LogFlow.Viewer
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;

    internal static class FileExtensionModule
    {
        public static string GetFileExtension(IEnumerable<string> filePaths)
        {
            var extensions = filePaths.Select(p => Path.GetExtension(p)).Distinct();
            if (extensions.Count() != 1)
            {
                throw new InvalidOperationException("Not support opening files with mixed types for now.");
                // TODO: We should support opening multiple files with mixed file types.
            }

            return extensions.FirstOrDefault();
        }
    }
}
