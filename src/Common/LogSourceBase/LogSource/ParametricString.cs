﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public class ParametricString
    {
        private string storedString;
        private const string LeftSign = "{{{";
        private const string RightSign = "}}}";

        public ParametricString(string template, params object[] parameters)
        {
            this.storedString = string.Intern(string.Format(template, parameters.Select(p => $"{LeftSign}{p}{RightSign}").ToArray()));
        }

        public IEnumerable<KeyValuePair<string, bool>> GetTokens()
        {
            //todo : improve the perf for drawing.
            int p = 0;
            bool isParameter = false;

            while (p < this.storedString.Length)
            {
                int l = this.storedString.IndexOf(LeftSign, p);
                if (l == -1) l = this.storedString.Length;
                int r = this.storedString.IndexOf(RightSign, p);
                if (r == -1) r = this.storedString.Length;

                int next = Math.Min(l, r);
                if (next > p)
                {
                    yield return new KeyValuePair<string, bool>(this.storedString.Substring(p, next - p), isParameter);
                }

                isParameter = l < r;
                p = next + LeftSign.Length;
            }
        }
    }
}
