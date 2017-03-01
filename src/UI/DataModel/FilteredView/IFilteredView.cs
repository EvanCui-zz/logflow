using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LogFlow.DataModel
{
    public interface IFilteredView<out T> where T : DataItemBase
    {
        #region Find, Count, Filter, Tag features.

        /// <summary>
        /// Tag or replace the tag's filter.
        /// </summary>
        /// <param name="index">the tag index</param>
        /// <param name="filter">the filter</param>
        void Tag(int index, Filter filter);
        void UnTag(int index);

        /// <summary>
        /// Find the next occurrence. Yielding progress from 0 to 100, if -1 is yielded, it means no result till the end of the current direction.
        /// Continue iterating the progress will search from the other end. If -2 is yielded, it means no result for all items.
        /// </summary>
        /// <param name="filter"></param>
        /// <param name="currentIndex"></param>
        /// <param name="direction"></param>
        /// <param name="result"></param>
        /// <returns></returns>
        IEnumerable<int> Find(Filter filter, int currentIndex, bool direction);

        IEnumerable<int> Count(Filter filter);

        IEnumerable<int> Initialize();

        bool IsInitialized { get; }

        #endregion

        #region Events

        event EventHandler<int> ItemAdded;

        #endregion

        #region Child

        IFilteredView<T> CreateChild(Filter filter);

        #endregion

        #region Display

        event EventHandler<ProgressItem> ProgressChanged;
        int? FirstDisplayedScrollingRowIndex { get; set; }
        int? SelectedRowIndex { get; set; }
        int? LastCountResult { get; }

        string Name { get; }

        int TotalCount { get; }

        object GetColumnValue(int rowIndex, int columnIndex);

        T GetRowValue(int rowIndex);

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
