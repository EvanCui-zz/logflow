namespace LogFlow.DataModel.Algorithm
{
    using System;
    using System.Runtime.Caching;

    public static class LocalStringPool
    {
        private static readonly CacheItemPolicy Sliding2Munites = new CacheItemPolicy() { SlidingExpiration = TimeSpan.FromMinutes(2) };

        private static readonly MemoryCache StringCache = new MemoryCache("StringCache");

        public static string Intern(string str)
        {
            var newval = StringCache.AddOrGetExisting(str, str, Sliding2Munites) as string;

            return newval ?? str;
        }
    }
}
