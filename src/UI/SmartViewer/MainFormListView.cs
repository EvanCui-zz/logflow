using System.Drawing.Drawing2D;
using System.Drawing.Text;

namespace LogFlow.Viewer
{
    using LogFlow.DataModel;
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Diagnostics;
    using System.Drawing;
    using System.Globalization;
    using System.IO;
    using System.Linq;
    using System.Reflection;
    using System.Threading;
    using System.Windows.Forms;
    using LogFlow.Viewer.Properties;

    public partial class MainFormListView : Form
    {
        private StringFormat DefaultStringFormat { get; set; }

        private CancellationTokenSource cts;

        private IList<SolidBrush> TagBrushes =>
            this.tagBrushes ?? (this.tagBrushes = new List<SolidBrush>()
            {
                new SolidBrush(Settings.Default.Display_TagColors.Tag1),
                new SolidBrush(Settings.Default.Display_TagColors.Tag2),
                new SolidBrush(Settings.Default.Display_TagColors.Tag3),
            });

        private List<SolidBrush> tagBrushes;

        private void RefreshTagColors()
        {
            this.tagBrushes?.ForEach(b => b.Dispose());
            this.tagBrushes = null;

            this.toolStripSplitButtonTag.DropDownItems.Clear();

            var i = 0;
            this.toolStripSplitButtonTag.DropDownItems.AddRange(this.TagBrushes.Select(t => (ToolStripItem)new ToolStripMenuItem($"Tag {++i}", null, (s, e1) =>
            {
                int index = int.Parse(((ToolStripMenuItem)s).Text.Substring(4)) - 1;
                if (this.CurrentView == null) return;
                var currentMenuItem = (ToolStripMenuItem)s;
                bool tag = !string.IsNullOrEmpty(this.toolStripComboBoxString.Text);
                currentMenuItem.Checked = tag;
                this.TagCurrentView(index, tag ? new Filter(this.toolStripComboBoxString.Text) : null);
            })
            {
                BackColor = t.Color,
            }).ToArray());

            this.toolStripSplitButtonTag.DefaultItem = this.toolStripSplitButtonTag.DropDownItems[0];
        }

        public MainFormListView()
        {
            InitializeComponent();

            Settings.Default.Display_TagColors = Settings.Default.Display_TagColors ?? new TagColors();
            Settings.Default.Display_LevelColors = Settings.Default.Display_LevelColors ?? new LevelColors();

            HotKeys.KeyActions = new Dictionary<string, Action>()
            {
                { HotKeys.ActionFocusPatternBox, () => this.toolStripComboBoxString.Focus() },
                { HotKeys.ActionGoto, () => this.toolStripComboBoxString.Focus() },
                { HotKeys.ActionFilter, () => this.toolStripButtonFilter_Click(this, null) },
                { HotKeys.ActionSearch, () => this.findNextToolStripMenuItem_Click(this, null) },
                { HotKeys.ActionCount, () => this.toolStripButtonCount_Click(this, null) },
                { HotKeys.ActionTag, () => this.toolStripSplitButtonTag.DefaultItem.PerformClick() },
                { HotKeys.ActionOpen, () => this.openToolStripMenuItem_Click(this, null) },
                { HotKeys.ActionSearchOpen, () => this.filteredOpenToolStripMenuItem_Click(this, null) },
            };
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.cts = new CancellationTokenSource();
            this.timerMemory.Start();
            this.fastListViewMain.SelectionForeColorBrush = new SolidBrush(this.fastListViewMain.SelectionForeColor);
            this.fastListViewMain.SelectionBackColorBrush = new SolidBrush(this.fastListViewMain.SelectionBackColor);
            this.fastListViewMain.AlternateBackColorBrush = new SolidBrush(this.fastListViewMain.AlternateBackColor);
            this.fastListViewMain.GridLineColorPen = new Pen(this.fastListViewMain.GridLineColor);
            this.fastListViewMain.SetHeight(21);

            this.DefaultStringFormat = new StringFormat(StringFormatFlags.NoWrap)
            {
                Trimming = StringTrimming.EllipsisCharacter,
                LineAlignment = StringAlignment.Center,
            };

            this.toolStripSplitButtonFind.DefaultItem = this.findNextToolStripMenuItem;
            this.ApplySettings();

            this.toolStripComboBoxString_TextChanged(this, null);
        }

        private void ApplySettings()
        {
            this.showFilenameColumnToolStripMenuItem.Checked = !Settings.Default.Display_HidenColumns.Contains("File");
            this.showLevelColumnToolStripMenuItem.Checked = !Settings.Default.Display_HidenColumns.Contains("Level");
            this.showActivityColumnToolStripMenuItem.Checked = !Settings.Default.Display_HidenColumns.Contains("ActId");
            this.boldParametersToolStripMenuItem.Checked = Settings.Default.Display_BoldParameter;
            this.highlightLevelsToolStripMenuItem.Checked = Settings.Default.Display_ColoredLevel;
            this.showStatisticsToolStripMenuItem.Checked = this.splitContainerStatistics.Visible = Settings.Default.Display_Statistics;
            this.fastRenderingToolStripMenuItem.Checked = Settings.Default.Display_FastRendering;
            this.autoScrollToolStripMenuItem.Checked = Settings.Default.Behavior_AutoScroll;
            this.detectNewLogsToolStripMenuItem.Checked = Settings.Default.Behavior_AutoLoad;
            this.enableDataVirtualizationToolStripMenuItem.Checked = Settings.Default.Behavior_DataVirtualization;
            this.RefreshTagColors();
            this.fastListViewMain.ReloadSetting();
            this.SaveScrollAndSelected();
            this.UpdateFastListViewColumns();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                this.OpenFiles(this.openFileDialog1.FileNames);
            }
        }

        private void OpenFiles(IEnumerable<string> filePaths, IFilter filter = null)
        {
            var document = new RootView<DataItemBase>(LogSourceManager.Instance.GetLogSource(string.Join(",", filePaths)), filter);
            this.AddView(document, true, true);
        }

        public IFilteredView<DataItemBase> CurrentView { get; set; }

        #region TreeView

        private void treeViewDoc_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.CurrentView = e.Node.Tag as FilteredView<DataItemBase>;

            this.UpdateDocDisplay();

            if (this.CurrentView == null)
            {
                this.closeToolStripMenuItem.Enabled = false;
                this.filterToolStripMenuItemDoc.Enabled = false;
                this.Text = Product.GetTitle();
                return;
            }

            this.Text = $"{Product.GetTitle()} - {this.CurrentView.Name}";

            this.closeToolStripMenuItem.Enabled = true;
            this.filterToolStripMenuItemDoc.Enabled = true;

            this.ChildView_ProgressChanged(this.CurrentView, this.CurrentView.CurrentProgress);

            if (!this.CurrentView.IsInitialized && !this.CurrentView.IsInProgress)
            {
                var bw = new BackgroundWorker { WorkerReportsProgress = true };
                var watch = Stopwatch.StartNew();

                bw.RunWorkerCompleted += (s, e1) =>
                {
                    if (this.IsDisposed) return;
                    this.UpdateStatistics();

                    watch.Stop();
                    this.toolStripStatusLabel1.Text = $"Used: {watch.Elapsed}";

                    bw.Dispose();
                };

                bw.ProgressChanged += (s, e1) =>
                {
                    // uninitialized view doesn't fire event by design, for better UI performance, so we need update this.
                    this.UpdateMainGridRowCount(e1.UserState, this.CurrentView.TotalCount);
                };

                bw.DoWork += (o, args) =>
                {
                    var view = this.CurrentView;
                    if (view == null) return;
                    foreach (var progress in view.Initialize(this.cts.Token))
                    {
                        bw.ReportProgress(progress, view);
                    }
                };

                bw.RunWorkerAsync();
            }
            else
            {
                if (this.CurrentView.LastCountResult.HasValue)
                    this.toolStripLabelCount.Text = this.CurrentView.LastCountResult.Value.ToString();
            }
        }

        private void treeViewDoc_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            this.SaveScrollAndSelected();
        }

        #endregion

        private void SaveScrollAndSelected()
        {
            if (this.CurrentView == null) return;
            this.CurrentView.FirstDisplayedScrollingRowIndex = this.fastListViewMain.TopItem?.Index;
            if (this.fastListViewMain.SelectedIndices.Count > 0)
            {
                this.CurrentView.SelectedRowIndex = this.fastListViewMain.SelectedIndices[0];
            }
            else
            {
                this.CurrentView.SelectedRowIndex = null;
            }
        }

        private void UpdateMainGridRowCount(object sender, int index)
        {
            if (object.ReferenceEquals(sender, this.CurrentView) && this.CurrentView.TotalCount > this.fastListViewMain.VirtualListSize && this.fastListViewMain.VirtualListSize < 100000000)
            {
                if (this.CurrentView.TotalCount > 100000000)
                {
                    MessageBox.Show(
                        Resources.TooManyRowsText,
                        Resources.TooManyRowsTitle,
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Exclamation);
                    this.fastListViewMain.VirtualListSize = 100000000;
                }
                else
                {
                    this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                }
            }
        }

        private void ChildView_ProgressChanged(object sender, ProgressItem e)
        {
            if (this.IsDisposed) return;
            this.Invoke(new Action(() =>
            {
                try
                {
                    if (!object.ReferenceEquals(sender, this.CurrentView)) return;
                    this.progressBarMain.Value = e.Progress;
                    this.toolStripStatusLabel.Text = e.ProgressDescription;
                    this.progressBarMain.Visible = this.CurrentView.IsInProgress;
                    this.UpdateSelectedStatus();
                    Application.DoEvents();
                }
                catch (ObjectDisposedException) { }
            }));
        }

        #region Filter, Tag, Find, Count

        private void filterToolStripMenuItemDoc_Click(object sender, EventArgs e)
        {
            MessageBox.Show(Resources.NotImplementedText, Resources.NotImplementedTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void TagCurrentView(int index, IFilter filter)
        {
            if (this.CurrentView == null) return;

            if (filter == null)
            {
                this.CurrentView.UnTag(index);
            }
            else
            {
                this.CurrentView.Tag(index, filter);
            }

            this.fastListViewMain.Refresh();
        }

        private void toolStripButtonFilter_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;

            var childView = this.CurrentView.CreateChild(new Filter(this.toolStripComboBoxString.Text));
            this.AddView(childView);
        }

        private void UpdateDocDisplay()
        {
            this.UpdateFastListViewColumns();
            this.UpdateDetailedPane();
            this.UpdateStatistics();
        }

        private void UpdateStatistics()
        {
            var stat = this.CurrentView?.Statistics;
            if (stat != null && !this.IsDisposed && !(this.propertyGridStatistics?.IsDisposed ?? true))
            {
                this.propertyGridStatistics.SelectedObject = stat;
                this.listViewExceptions.Items.AddRange(
                    this.CurrentView.Statistics.Exceptions.Select(kvp =>
                    {
                        var item = new ListViewItem(kvp.Key);
                        item.SubItems.Add(new ListViewItem.ListViewSubItem() { Text = kvp.Value.ToString() });
                        return item;
                    }).ToArray());

                this.chartTimeLine.Series.Clear();
                var series = this.chartTimeLine.Series.Add("timeline");
                if (this.CurrentView.IsInitialized)
                {
                    foreach (var y in stat.Timeline)
                    {
                        series.Points.AddY(y);
                    }
                }
            }
        }

        private int[] columnDataIndexMapping;

        private void UpdateFastListViewColumns()
        {
            var view = this.CurrentView;

            this.fastListViewMain.BeginUpdate();
            this.fastListViewMain.Columns.Clear();
            this.fastListViewMain.Items.Clear();
            this.fastListViewMain.VirtualListSize = 0;

            if (view != null)
            {
                this.columnDataIndexMapping = new int[view.ColumnInfos.Count + 1];

                var p = 1;

                var headers = new List<ColumnHeader> { new ColumnHeader() { Name = "Workaround", Text = "", Width = 0 } };

                for (var i = 0; i < view.ColumnInfos.Count; i++)
                {
                    var info = view.ColumnInfos[i];

                    if (!Settings.Default.Display_HidenColumns?.Contains(info.Name) ?? true)
                    {
                        headers.Add(new ColumnHeader() { Name = info.Name, Text = info.Name, Width = info.Width });
                        this.columnDataIndexMapping[p++] = i;
                    }
                }

                this.fastListViewMain.Columns.AddRange(headers.ToArray());
                this.fastListViewMain_Resize(this, null);
                this.fastListViewMain.VirtualListSize = view.TotalCount;
                if (view.SelectedRowIndex.HasValue)
                    this.fastListViewMain.Items[view.SelectedRowIndex.Value].Selected = true;
                if (view.FirstDisplayedScrollingRowIndex.HasValue)
                    this.fastListViewMain.TopItem = this.fastListViewMain.Items[view.FirstDisplayedScrollingRowIndex.Value];
            }

            this.fastListViewMain.EndUpdate();
        }

        private void AddView(IFilteredView<DataItemBase> childView, bool activateView = true, bool toRoot = false)
        {
            childView.ItemAdded += this.UpdateMainGridRowCount;
            childView.ProgressChanged += ChildView_ProgressChanged;

            TreeNode node;
            if (this.treeViewDoc.Nodes.Count == 0 || toRoot)
            {
                node = this.treeViewDoc.Nodes.Add(childView.Name, childView.Name);
            }
            else
            {
                node = this.treeViewDoc.SelectedNode.Nodes.Add(childView.Name, childView.Name);
            }

            node.Tag = childView;

            if (activateView)
            {
                this.treeViewDoc.SelectedNode = node;
            }

            if (childView.GroupFilters != null)
            {
                foreach (var v in childView.GroupFilters.Select(childView.CreateChild).ToList())
                {
                    this.AddView(v, false);
                }
            }

            if (!node.IsExpanded) node.Expand();
        }

        private void RemoveView(IFilteredView<DataItemBase> view)
        {
            view.ItemAdded -= this.UpdateMainGridRowCount;
            view.ProgressChanged -= this.ChildView_ProgressChanged;

            var disposable = view as IDisposable;

            disposable?.Dispose();
        }

        private void findPreviousToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int currentIndex = this.fastListViewMain.VirtualListSize - 1;
            if (this.fastListViewMain.SelectedIndices.Count > 0)
            {
                currentIndex = this.fastListViewMain.SelectedIndices[0];
            }

            this.Find(currentIndex, false);
        }

        private void findNextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int currentIndex = 0;
            if (this.fastListViewMain.SelectedIndices.Count > 0)
            {
                currentIndex = this.fastListViewMain.SelectedIndices[this.fastListViewMain.SelectedIndices.Count - 1];
            }

            this.Find(currentIndex, true);
        }

        private void Find(int startIndex, bool direction)
        {
            if (this.CurrentView == null) return;
            var f = new Filter(this.toolStripComboBoxString.Text);

            var bw = new BackgroundWorker();

            bw.RunWorkerCompleted += (s, e1) =>
            {
                if (this.IsDisposed) return;
                var result = (Tuple<IFilteredView<DataItemBase>, int?>)e1.Result;
                if (object.ReferenceEquals(result.Item1, this.CurrentView))
                {
                    if (result.Item2.HasValue)
                    {
                        this.fastListViewMain.SelectedIndices.Clear();
                        this.fastListViewMain.Items[result.Item2.Value].Selected = true;
                        this.fastListViewMain.Items[result.Item2.Value].EnsureVisible();
                        this.toolStripLabelCount.Text = result.Item2.ToString();
                    }
                }

                bw.Dispose();
            };

            bw.DoWork += (s, e1) =>
            {
                var currentView = this.CurrentView;
                foreach (var _ in currentView.Find(f, startIndex, direction))
                {
                }

                e1.Result = Tuple.Create(currentView, currentView.SelectedRowIndex);
            };

            bw.RunWorkerAsync();
        }

        private void toolStripButtonCount_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;
            var f = new Filter(this.toolStripComboBoxString.Text);

            var bw = new BackgroundWorker();

            bw.RunWorkerCompleted += (s, e1) =>
            {
                if (this.IsDisposed) return;
                this.toolStripLabelCount.Text = e1.Result.ToString();

                bw.Dispose();
            };

            bw.DoWork += (s, e1) =>
            {
                var currentView = this.CurrentView;
                foreach (var _ in currentView.Count(f))
                {
                }

                e1.Result = currentView.LastCountResult ?? 0;
            };

            bw.RunWorkerAsync();
        }

        #endregion

        private void fastListViewMain_CacheVirtualItems(object sender, CacheVirtualItemsEventArgs e)
        {
            // todo, enable cache when we do data virtualization.
        }

        private void fastListViewMain_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            e.Item = new ListViewItem();
            if (this.CurrentView == null) { return; }
            var item = this.CurrentView.GetRowValue(e.ItemIndex);

            e.Item.Text = "";
            e.Item.Tag = item;

            var count = this.CurrentView.ColumnInfos.Count;
            var subItems = new ListViewItem.ListViewSubItem[count];

            for (int i = 0; i < count; i++)
            {
                subItems[i] = new ListViewItem.ListViewSubItem();
            }

            subItems[3] = new ListViewItem.ListViewSubItem() { Tag = item == null ? null : this.CurrentView.GetColumnValue(e.ItemIndex, 3) };
            subItems[4] = new ListViewItem.ListViewSubItem() { Tag = item == null ? null : this.CurrentView.GetColumnValue(e.ItemIndex, 4) };

            e.Item.SubItems.AddRange(subItems);
        }

        private void fastListViewMain_DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            if (e.ItemIndex == -1) return;
            Debug.WriteLine("listview drawItem {0}", e.ItemIndex);

            var bound = e.Bounds;
            DataItemBase item = (DataItemBase)e.Item.Tag;
            bool isSelected = e.Item.Selected;

            // draw background
            if (isSelected)
            {
                // normal background
                e.Graphics.FillRectangle(e.ItemIndex % 2 == 1 ? this.fastListViewMain.AlternateBackColorBrush : this.fastListViewMain.SelectionBackColorBrush, bound);
            }
            else
            {
                // level background
                if (Settings.Default.Display_ColoredLevel)
                {
                    var level = item.Level;
                    var index = level == LogLevels.Critical
                        ? 0
                        : (level == LogLevels.Error
                            ? 1
                            : (level == LogLevels.Warning
                                ? 2
                                : (level == LogLevels.Info
                                    ? 3
                                    : (level == LogLevels.Verbose ? 4 : 5))));

                    if (index < this.fastListViewMain.LevelBrushes.Count)
                    {
                        e.Graphics.FillRectangle(this.fastListViewMain.LevelBrushes[index], bound);
                    }
                    else
                    {
                        e.DrawBackground();
                    }
                }
                else
                {
                    e.DrawBackground();
                }
            }

            // draw lines
            e.Graphics.DrawLine(this.fastListViewMain.GridLineColorPen, bound.X, bound.Y + bound.Height - 1, bound.X + bound.Width, bound.Y + bound.Height - 1);

            e.DrawFocusRectangle();

        }

        private void fastListViewMain_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            //    Debug.WriteLine("Redrawing Row {0} Column {1}", e.ItemIndex, e.ColumnIndex);
            // draw contents.
            var bound = e.Bounds;
            DataItemBase item = (DataItemBase)e.Item.Tag;
            if (item == null) return;
            var isSelected = e.Item.Selected;
            StringFormat format = new StringFormat(StringFormatFlags.NoWrap)
            {
                //   Alignment = StringAlignment.Center,
                Trimming = StringTrimming.EllipsisCharacter,
                LineAlignment = StringAlignment.Center,
            };

            if (Settings.Default.Display_FastRendering)
            {
                e.Graphics.SmoothingMode = SmoothingMode.HighSpeed;
                e.Graphics.TextRenderingHint = TextRenderingHint.SingleBitPerPixel;
            }

            Brush foreBrush = isSelected ? this.fastListViewMain.SelectionForeColorBrush : this.fastListViewMain.ForeColorBrush;

            switch (e.ColumnIndex)
            {
                case 5:
                    bool isIndented = this.CurrentView?.IsThreadIndented(item.ThreadId) ?? false;
                    if (isIndented)
                    {
                        bound.Width -= 100;
                        bound.X += 100;
                    }

                    var paramString = (ParametricString)e.SubItem.Tag;

                    foreach (var token in paramString.GetTokens())
                    {
                        const int multiLineSignWidth = 25;
                        var str = token.Key;
                        var pos = str.IndexOf(Environment.NewLine, StringComparison.Ordinal);
                        if (pos >= 0)
                        {
                            str = str.Substring(0, pos);
                            bound.Width -= multiLineSignWidth;
                        }

                        var currentFont = token.Value && Settings.Default.Display_BoldParameter ? this.fastListViewMain.BoldFont : this.fastListViewMain.NormalFont;

                        if (str.Length > 0)
                        {
                            e.Graphics.DrawString(
                                str,
                                currentFont,
                                isSelected ? this.fastListViewMain.SelectionForeColorBrush : this.fastListViewMain.ForeColorBrush,
                                bound,
                                this.DefaultStringFormat);
                        }

                        if (pos > 0)
                        {
                            bound = e.Bounds;
                            bound.X += bound.Width - multiLineSignWidth;
                            bound.Width = multiLineSignWidth;

                            // draw a >> sign to indicate multiline
                            e.Graphics.DrawString(
                                "↓↓↓",
                                currentFont,
                                Brushes.Moccasin,
                                bound,
                                this.DefaultStringFormat);

                            break;
                        }

                        var length = e.Graphics.MeasureString(token.Key, currentFont).Width + 0.5f;
                        bound.Width -= (int)length;
                        if (bound.Width <= 0) break;
                        bound.X += (int)length;
                    }
                    break;

                case 4:
                    var colorList = (List<int>)e.SubItem.Tag;

                    var rect = e.Bounds;
                    rect.X += 4;
                    rect.Y += 4;
                    rect.Height -= 8;
                    rect.Width = rect.Height;

                    int p = 0;

                    for (int j = 0; j < this.TagBrushes.Count; j++)
                    {
                        if (p < colorList.Count && colorList[p] == j)
                        {
                            e.Graphics.FillRectangle(this.TagBrushes[j], rect);
                            p++;
                        }

                        rect.X += rect.Width + 2;
                    }
                    break;

                default:
                    e.Graphics.DrawString(this.CurrentView?.GetColumnValue(e.ItemIndex, this.columnDataIndexMapping[e.ColumnIndex]).ToString(), e.SubItem.Font, foreBrush, bound, format);
                    break;
            }
        }

        private void fastListViewMain_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            e.DrawBackground();
            e.DrawText(TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
            var bound = e.Bounds;
            e.Graphics.DrawLine(this.fastListViewMain.GridLineColorPen, bound.X, bound.Y + bound.Height - 1, bound.X + bound.Width, bound.Y + bound.Height - 1);
        }

        private void toolStripComboBoxString_TextChanged(object sender, EventArgs e)
        {
            bool enabled = !string.IsNullOrEmpty(this.toolStripComboBoxString.Text) && this.CurrentView != null;
            this.toolStripButtonFilter.Enabled = enabled;
            this.toolStripButtonCount.Enabled = enabled;
            this.toolStripSplitButtonFind.Enabled = enabled;
            this.toolStripSplitButtonTag.Enabled = enabled;
        }

        private void fastListViewMain_Resize(object sender, EventArgs e)
        {
            var textWidth = this.fastListViewMain.Width - 1;
            ColumnHeader textCol = null;

            foreach (ColumnHeader col in this.fastListViewMain.Columns)
            {
                if (textCol == null && string.Equals(col.Name, "Text"))
                {
                    textCol = col;
                }
                else
                {
                    textWidth -= col.Width;
                }
            }

            if (textCol != null)
            {
                textCol.Width = textWidth < 0 ? 10 : textWidth;
            }
        }

        private void UpdateDetailedPane()
        {
            if (this.fastListViewMain.SelectedIndices.Count > 0)
            {
                var firstSelectedIndex = this.fastListViewMain.SelectedIndices[0];
                var item = (DataItemBase)this.fastListViewMain.Items[firstSelectedIndex].Tag;
                this.labelId.Text = item.Id.ToString();
                this.labelTime.Text = item.Time.ToString(CultureInfo.InvariantCulture);
                this.labelThreadId.Text = item.ThreadId.ToString();
                this.labelProcessId.Text = item.ProcessId.ToString();
                this.labelLevel.Text = item.Level.ToString();
                this.textBoxText.Text = string.Format(this.CurrentView.Templates[item.TemplateId], item.Parameters);
            }
            else
            {
                this.labelId.Text = this.labelTime.Text = this.labelThreadId.Text = this.labelProcessId.Text = this.labelLevel.Text = this.textBoxText.Text = string.Empty;
            }
        }

        private void fastListViewMain_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.UpdateDetailedPane();
            UpdateSelectedStatus();
        }

        private void UpdateSelectedStatus()
        {
            var firstSelectedIndex = this.fastListViewMain.SelectedIndices.Count > 0
                ? this.fastListViewMain.SelectedIndices[0]
                : -1;

            this.toolStripStatusLabelSelected.Text = $"{firstSelectedIndex}, {this.CurrentView.TotalCount}";
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.CloseCurrentView();
        }

        private void CloseCurrentView()
        {
            var nodeNext = this.treeViewDoc.SelectedNode?.Parent ?? this.treeViewDoc.SelectedNode?.PrevNode ?? this.treeViewDoc.SelectedNode?.NextNode;
            var currentView = this.CurrentView;
            this.treeViewDoc.SelectedNode?.Remove();

            // Remove children.
            this.RemoveView(currentView);

            this.treeViewDoc.SelectedNode = nodeNext;
        }

        private void openToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.openToolStripMenuItem_Click(sender, e);
        }

        private void filterWithTheSameThreadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.fastListViewMain.SelectedIndices.Count == 0 || this.CurrentView == null)
            {
                return;
            }

            var threadId = this.CurrentView.GetRowValue(this.fastListViewMain.SelectedIndices[0]).ThreadId;

            var f = new Filter($"t:{threadId}");
            var childView = this.CurrentView.CreateChild(f);

            this.AddView(childView);
        }

        private void indentTheThreadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.fastListViewMain.SelectedIndices.Count == 0 || this.CurrentView == null)
            {
                return;
            }

            int threadId = this.CurrentView.GetRowValue(this.fastListViewMain.SelectedIndices[0]).ThreadId;

            this.CurrentView.IndentThread(threadId);
            this.fastListViewMain.Refresh();
        }

        private void unindentAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.CurrentView?.UnIndentAll();
            this.fastListViewMain.Refresh();
        }

        private void unindentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.fastListViewMain.SelectedIndices.Count == 0 || this.CurrentView == null)
            {
                return;
            }

            int threadId = this.CurrentView.GetRowValue(this.fastListViewMain.SelectedIndices[0]).ThreadId;

            this.CurrentView.UnIndentThread(threadId);
            this.fastListViewMain.Refresh();
        }

        private void findTheLineFromParentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.FindInNode(this.treeViewDoc.SelectedNode?.Parent);
        }

        private void goToToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.FindInNode(this.treeViewDoc.Nodes[0]);
        }

        private void FindInNode(TreeNode node)
        {
            if (this.fastListViewMain.SelectedIndices.Count == 0 || this.CurrentView == null || node == null)
            {
                return;
            }

            var itemId = this.CurrentView.GetRowValue(this.fastListViewMain.SelectedIndices[0]).Id;

            this.treeViewDoc.SelectedNode = node;

            // the item can always be found.
            this.GotoId(itemId);
        }

        private bool GotoId(int physicalId)
        {
            var logicalId = this.CurrentView.GetLogicalIndexOfItem(physicalId);

            if (logicalId < 0) return false;
            this.fastListViewMain.Items[logicalId].Selected = this.fastListViewMain.Items[logicalId].Focused = true;
            this.fastListViewMain.Items[logicalId].EnsureVisible();

            return true;
        }

        private void timerMemory_Tick(object sender, EventArgs e)
        {
            var memoryMb = ((double)GC.GetTotalMemory(false) / 1024) / 1024;
            this.toolStripStatusLabelMemory.Text = $"{memoryMb:0.##}MB";
        }

        private bool resizeGuide;

        private void fastListViewMain_ColumnWidthChanged(object sender, ColumnWidthChangedEventArgs e)
        {
            if (!this.resizeGuide)
            {
                this.resizeGuide = true;
                this.fastListViewMain_Resize(sender, null);
                this.resizeGuide = false;
            }
        }

        private void FilterById(bool isBegin)
        {
            if (this.fastListViewMain.SelectedIndices.Count == 0 || this.CurrentView == null)
            {
                return;
            }

            var id = this.CurrentView.GetRowValue(this.fastListViewMain.SelectedIndices[0]).Id;

            var childView = this.CurrentView.CreateChild(Filter.CreateFilter((item, template) => isBegin == (item.Id >= id)));
            this.AddView(childView);
        }

        private void filterAsStartToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.FilterById(true);
        }

        private void filterAsEndToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.FilterById(false);
        }

        private void MainFormListView_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.cts?.Cancel();
        }

        private void versionHistoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var versionHistoryFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\Docs\\VersionHistory.html";
            if (File.Exists(versionHistoryFile))
            {
                Process.Start(versionHistoryFile);
            }
            else
            {
                MessageBox.Show(Resources.VersionFileMissingText, Resources.SomethingWrong, MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation);
            }
        }

        private void MainFormListView_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                var filePaths = (string[])e.Data.GetData(DataFormats.FileDrop);

                this.OpenFiles(filePaths);
            }
        }

        private void MainFormListView_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                var filePaths = (string[])e.Data.GetData(DataFormats.FileDrop);
                if (filePaths.Length > 0) e.Effect = DragDropEffects.Move;
            }
        }

        private void MainFormListView_KeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = HotKeys.Instance.PerformAction(e.KeyData);
        }

        private void closeToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.CloseCurrentView();
        }

        private void filteredOpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var searchFileDialog = new SearchFileDialog())
            {
                var result = searchFileDialog.ShowDialog();
                switch (result)
                {
                    case DialogResult.OK:
                        // all matched;
                        this.OpenFiles(searchFileDialog.MatchedFilePaths);
                        break;
                    case DialogResult.Yes:
                        this.OpenFiles(searchFileDialog.SelectedFilePaths);
                        break;
                    case DialogResult.Retry:
                        this.OpenFiles(searchFileDialog.MatchedFilePaths, searchFileDialog.Filter);
                        break;
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void allSettingsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var allSettings = new AllSettings())
            {
                if (DialogResult.OK == allSettings.ShowDialog())
                {
                    // for apply button case, we update each time.
                    this.ApplySettings();
                }
            }
        }

        private void boldParametersToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Display_BoldParameter = this.boldParametersToolStripMenuItem.Checked;
            this.fastListViewMain.Refresh();
            Settings.Default.Save();
        }

        private void showLevelColumnToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.SetColumnVisible("Level", this.showLevelColumnToolStripMenuItem.Checked);
        }

        private void SetColumnVisible(string name, bool visible)
        {
            if (!visible & !Settings.Default.Display_HidenColumns.Contains(name))
            {
                Settings.Default.Display_HidenColumns.Add(name);
            }

            if (visible && Settings.Default.Display_HidenColumns.Contains(name))
            {
                Settings.Default.Display_HidenColumns.Remove(name);
            }

            Settings.Default.Save();

            this.SaveScrollAndSelected();
            this.UpdateFastListViewColumns();
        }

        private void showFilenameColumnToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.SetColumnVisible("File", this.showFilenameColumnToolStripMenuItem.Checked);
        }

        private void showActivityColumnToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.SetColumnVisible("ActId", this.showActivityColumnToolStripMenuItem.Checked);
        }

        private void highlightLevelsToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Display_ColoredLevel = this.highlightLevelsToolStripMenuItem.Checked;
            this.fastListViewMain.Refresh();
        }

        private void showStatisticsToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            this.panelRight.Visible = Settings.Default.Display_Statistics = this.showStatisticsToolStripMenuItem.Checked;
        }

        private void fastRenderingToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Display_FastRendering = this.fastRenderingToolStripMenuItem.Checked;
            this.fastListViewMain.Refresh();
        }

        private void detectNewLogsToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Behavior_AutoLoad = this.detectNewLogsToolStripMenuItem.Checked;
        }

        private void autoScrollToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Behavior_AutoScroll = this.autoScrollToolStripMenuItem.Checked;
        }

        private void enableDataVirtualizationToolStripMenuItem_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.Behavior_DataVirtualization = this.enableDataVirtualizationToolStripMenuItem.Checked;
        }

        private void toolStripComboBoxString_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode != Keys.Enter || this.CurrentView == null) return;

            int gotoId;
            if (!int.TryParse(this.toolStripComboBoxString.Text, out gotoId))
            {
                return;
            }

            if (!this.GotoId(gotoId))
            {
                MessageBox.Show(string.Format(Resources.GotoFailed, gotoId), Resources.SomethingWrong,
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
