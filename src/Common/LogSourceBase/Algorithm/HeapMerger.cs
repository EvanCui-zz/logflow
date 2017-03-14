namespace LogFlow.DataModel.Algorithm
{
    using System.Collections.Generic;
    using System.Threading;

    public struct MergedItem<T>
    {
        public MergedItem(T item, int sourceIndex) { this.Item = item; this.SourceIndex = sourceIndex; }
        public T Item { get; }
        public int SourceIndex { get; }
    }

    public class HeapMerger
    {
        public static IEnumerable<MergedItem<T>> Merge<T>(CancellationToken token, IComparer<T> comparer = null, params IEnumerable<T>[] sources)
        {
            comparer = comparer ?? Comparer<T>.Default;
            return Merge(sources, 0, sources.Length - 1, comparer, token);
        }

        private static IEnumerable<MergedItem<T>> Merge<T>(IEnumerable<T>[] sources, int startIndex, int endIndex, IComparer<T> comparer, CancellationToken token)
        {
            if (endIndex == -1) yield break;
            if (endIndex == startIndex)
            {
                foreach (var item in sources[startIndex]) yield return new MergedItem<T>(item, startIndex);
            }
            else
            {
                var left = Merge(sources, startIndex, (startIndex + endIndex) / 2, comparer, token);
                var right = Merge(sources, (startIndex + endIndex) / 2 + 1, endIndex, comparer, token);

                using (var leftIt = left.GetEnumerator())
                using (var rightIt = right.GetEnumerator())
                {
                    if (token.IsCancellationRequested) yield break;
                    bool leftAvailable = leftIt.MoveNext();
                    bool rightAvailable = rightIt.MoveNext();
                    if (token.IsCancellationRequested) yield break;

                    while (leftAvailable && rightAvailable)
                    {
                        if (comparer.Compare(leftIt.Current.Item, rightIt.Current.Item) < 0)
                        {
                            yield return leftIt.Current;
                            leftAvailable = leftIt.MoveNext();
                            if (token.IsCancellationRequested) yield break;
                        }
                        else
                        {
                            yield return rightIt.Current;
                            rightAvailable = rightIt.MoveNext();
                            if (token.IsCancellationRequested) yield break;
                        }
                    }

                    IEnumerator<MergedItem<T>> lastIt = null;

                    if (leftAvailable) { yield return leftIt.Current; lastIt = leftIt; }
                    else if (rightAvailable) { yield return rightIt.Current; lastIt = rightIt; }

                    if (lastIt != null)
                    {
                        while (!token.IsCancellationRequested && lastIt.MoveNext()) yield return lastIt.Current;
                    }
                }
            }
        }
    }
}
