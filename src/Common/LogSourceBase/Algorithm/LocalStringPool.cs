namespace LogFlow.DataModel.Algorithm
{
    using System;
    using System.Runtime.Caching;

    internal static class LocalStringPool
    {
        private static readonly CacheItemPolicy Sliding2Munites = new CacheItemPolicy() { SlidingExpiration = TimeSpan.FromMinutes(2) };

        private static readonly MemoryCache StringCache = new MemoryCache("StringCache");

        public static string Intern(string str)
        {
            var newval = (string)StringCache.AddOrGetExisting(str, str, Sliding2Munites);
            //var newval = string.Intern(str);

        //      var newval = str;
            return newval ?? str;
        }
    }
}
