using System.Threading;

namespace LogFlow.DataModel
{
    using System;
    using System.Collections.Generic;
    using System.Threading.Tasks;

    public interface IFilteredView<out T> where T : DataItemBase
    {
        #region Find, Count, Filter, Tag, Indent features.

        int GetLogicalIndexOfItem(int itemId);

        void UnIndentActivity(int actIdIndex);

        void IndentActivity(int actIdIndex);

        bool IsActivityIndented(int actIdIndex);

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
        Task FindAsync(IFilter filter, int currentIndex, bool direction, CancellationToken token);

        Task CountAsync(IFilter filter, CancellationToken token);

        Task DoStatisticsAsync(CancellationToken token);

        #endregion

        #region Events

        event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        IFilteredView<T> CreateChild(IFilter filter);

        IReadOnlyList<IFilter> GroupFilters { get; }

        #endregion

        #region Display

        ILogSource<T> Source { get; }

        int? FirstDisplayedScrollingRowIndex { get; set; }
        int? SelectedRowIndex { get; set; }
        int? LastCountResult { get; }

        string Name { get; }

        int TotalCount { get; }

        object GetColumnValue(DataItemBase baseItem, int columnIndex);

        T GetRowValue(int rowIndex);

        int GetPhysicalIndex(int logicalIndex);

        IReadOnlyList<ColumnInfoAttribute> ColumnInfos { get; }

        // True if the view is in a progress of something
        bool IsInProgress { get; }
        ProgressItem CurrentProgress { get; }

        IReadOnlyList<string> Templates { get; }

        FilteredViewStatistics Statistics { get; }

        TimeSpan LastProgressUsedTime { get; }

        #endregion
    }
}
