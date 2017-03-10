using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;

    public interface IFilteredView
    {
        #region Find, Count, Filter, Tag, Indent features.

        int GetLogicalIndexOfItem(int itemId);

        void UnIndentThread(int threadId);

        void UnIndentAll();

        void IndentThread(int threadId);

        bool IsThreadIndented(int threadId);

        /// <summary>
        /// Tag or replace the tag's filter.
        /// </summary>
        /// <param name="index">the tag index</param>
        /// <param name="filter">the filter</param>
        void Tag(int index, IFilter filter);
        void UnTag(int index);
        bool IsTagged(int index);

        /// <summary>
        /// Find the next occurrence. Yielding progress from 0 to 100, if -1 is yielded, it means no result till the end of the current direction.
        /// Continue iterating the progress will search from the other end. If -2 is yielded, it means no result for all InternalItems.
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="currentIndex"></param>
        /// <param name="direction"></param>
        /// <returns></returns>
        IEnumerable<int> Find(IFilter filter, int currentIndex, bool direction);

        IEnumerable<int> Count(IFilter filter);

        IEnumerable<int> Initialize(bool statistics, CancellationToken token);

        bool IsInitialized { get; }

        #endregion

        #region Events

        event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        IFilteredView CreateChild(IFilter filter);

        IReadOnlyList<IFilter> GroupFilters { get; }

        #endregion

        #region Display

        event EventHandler<ProgressItem> ProgressChanged;
        int? FirstDisplayedScrollingRowIndex { get; set; }
        int? SelectedRowIndex { get; set; }
        int? LastCountResult { get; }

        string Name { get; }

        int TotalCount { get; }

        object GetColumnValue(int rowIndex, int columnIndex);

        DataItemStruct GetRowValue(int rowIndex);

        int GetPhysicalIndex(int logicalIndex);

        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }

        // True if the view is in a progress of something
        bool IsInProgress { get; }
        ProgressItem CurrentProgress { get; }

        IReadOnlyList<string> Templates { get; }

        FilteredViewStatistics Statistics { get; }

        #endregion
    }
}
