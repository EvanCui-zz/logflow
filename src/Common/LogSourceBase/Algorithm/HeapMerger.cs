using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel.Algorithm
{
    public struct MergedItem<T>
    {
        public MergedItem(T item, int sourceIndex) { this.Item = item; this.SourceIndex = sourceIndex; }
        public T Item { get; private set; }
        public int SourceIndex { get; private set; }
    }

    public class HeapMerger
    {
        public static IEnumerable<MergedItem<T>> Merge<T>(IComparer<T> comparer = null, params IEnumerable<T>[] sources)
        {
            comparer = comparer ?? Comparer<T>.Default;
            return Merge(sources, 0, sources.Length - 1, comparer);
        }

        private static IEnumerable<MergedItem<T>> Merge<T>(IEnumerable<T>[] sources, int startIndex, int endIndex, IComparer<T> comparer)
        {
            if (endIndex == -1) yield break;
            if (endIndex == startIndex)
            {
                foreach (var item in sources[startIndex]) yield return new MergedItem<T>(item, startIndex);
            }
            else
            {
                var left = Merge(sources, startIndex, (startIndex + endIndex) / 2, comparer);
                var right = Merge(sources, (startIndex + endIndex) / 2 + 1, endIndex, comparer);

                using (var leftIt = left.GetEnumerator())
                using (var rightIt = right.GetEnumerator())
                {
                    bool leftAvailable = leftIt.MoveNext();
                    bool rightAvailable = rightIt.MoveNext();

                    while (leftAvailable && rightAvailable)
                    {
                        if (comparer.Compare(leftIt.Current.Item, rightIt.Current.Item) < 0)
                        {
                            yield return leftIt.Current;
                            leftAvailable = leftIt.MoveNext();
                        }
                        else
                        {
                            yield return rightIt.Current;
                            rightAvailable = rightIt.MoveNext();
                        }
                    }

                    IEnumerator<MergedItem<T>> lastIt = null;

                    if (leftAvailable) { yield return leftIt.Current; lastIt = leftIt; }
                    else if (rightAvailable) { yield return rightIt.Current; lastIt = rightIt; }

                    if (lastIt != null)
                    {
                        while (lastIt.MoveNext()) yield return lastIt.Current;
                    }
                }
            }
        }
    }
}
