using DataModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;

namespace SmartViewer
{
    public partial class MainFormListView : Form
    {
        private LogDocument<DataItemBase> document;
        private StringFormat defaultStringFormat;

        private List<Tuple<Color, SolidBrush, Pen>> tags = new List<Tuple<Color, SolidBrush, Pen>>()
        {
            new Tuple<Color, SolidBrush, Pen>(Color.Cyan, new SolidBrush(Color.Cyan), new Pen(Color.Cyan)),
            new Tuple<Color, SolidBrush, Pen>(Color.FromArgb(128, 128, 255), new SolidBrush(Color.FromArgb(128, 128, 255)), new Pen(Color.FromArgb(128, 128, 255))),
            new Tuple<Color, SolidBrush, Pen>(Color.FromArgb(128, 255, 128), new SolidBrush(Color.FromArgb(128, 255, 128)), new Pen(Color.FromArgb(128, 255, 128))),
        };

        public MainFormListView()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            this.fastListViewMain.SelectionForeColorBrush = new SolidBrush(this.fastListViewMain.SelectionForeColor);
            this.fastListViewMain.SelectionBackColorBrush = new SolidBrush(this.fastListViewMain.SelectionBackColor);
            this.fastListViewMain.AlternateBackColorBrush = new SolidBrush(this.fastListViewMain.AlternateBackColor);
            this.fastListViewMain.GridLineColorPen = new Pen(this.fastListViewMain.GridLineColor);
            this.fastListViewMain.SetHeight(21);

            this.defaultStringFormat = new StringFormat(StringFormatFlags.NoWrap)
            {
                Trimming = StringTrimming.EllipsisCharacter,
                LineAlignment = StringAlignment.Center,
            };

            int i = 0;
            this.toolStripSplitButtonTag.DropDownItems.AddRange(this.tags.Select(t => new ToolStripMenuItem($"Tag {++i}", null, (s, e1) =>
            {
                int index = int.Parse(((ToolStripMenuItem)s).Text.Substring(4)) - 1;
                if (this.CurrentView == null) return;
                var currentMenuItem = (ToolStripMenuItem)s;
                bool tag = !string.IsNullOrEmpty(this.toolStripTextBoxPattern.Text);
                currentMenuItem.Checked = tag;
                this.TagCurrentView(index, tag ? new Filter(this.toolStripTextBoxPattern.Text) : null);
            })
            {
                BackColor = t.Item1,
            }).ToArray());

            this.toolStripSplitButtonTag.DefaultItem = this.toolStripSplitButtonTag.DropDownItems[0];
            this.toolStripSplitButtonFind.DefaultItem = this.findNextToolStripMenuItem;

            this.openToolStripMenuItem_Click(this, null);
            this.toolStripTextBoxPattern_TextChanged(this, null);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.document = new LogDocument<DataItemBase>("loaded test");
            this.treeViewDoc.Nodes.Clear();
            var node = this.treeViewDoc.Nodes.Add("Root", this.document.Name);
            this.document.ProgressChanged += ChildView_ProgressChanged;
            node.Tag = this.document;

            var columns = this.document.ColumnInfos.Select(ci => new DataGridViewTextBoxColumn()
            {
                Name = ci.Name,
                HeaderText = ci.Name,
                AutoSizeMode = string.Equals(ci.Name, "Text") ? DataGridViewAutoSizeColumnMode.Fill : DataGridViewAutoSizeColumnMode.None,
                MinimumWidth = 5,
                Width = ci.Width,
            }).ToArray();

            var headers = new ColumnHeader[1] { new ColumnHeader() { Name = "Workaround", Text = "", Width = 0 } }.Concat(this.document.ColumnInfos.Select(ci => new ColumnHeader()
            {
                Name = ci.Name,
                Text = ci.Name,
                Width = ci.Width,
            })).ToArray();

            this.fastListViewMain.Columns.Clear();
            this.fastListViewMain.Items.Clear();
            this.fastListViewMain.Columns.AddRange(headers);
            this.document.ItemAdded += this.UpdateMainGridRowCount;
            this.document.GenerateFakeData();
            this.treeViewDoc.SelectedNode = node;
        }

        public IFilteredView<DataItemBase> CurrentView { get; set; }

        #region TreeView

        private void treeViewDoc_AfterSelect(object sender, TreeViewEventArgs e)
        {
            this.CurrentView = e.Node.Tag as FilteredView<DataItemBase>;
            if (this.CurrentView == null)
            {
                return;
            }

            // *** DataGridView population ***
            // SendMessage(this.dataGridViewMain.Handle, WM_SETREDRAW, false, 0);
            Debug.WriteLine("Suspend layout");
            this.ChildView_ProgressChanged(this.CurrentView, this.CurrentView.CurrentProgress);
            this.fastListViewMain.VirtualListSize = 0;

            Debug.WriteLine("rows cleared");
            if (!this.CurrentView.IsInitialized && !this.CurrentView.IsInProgress)
            {
                var bw = new BackgroundWorker();
                bw.WorkerReportsProgress = true;

                bw.RunWorkerCompleted += (s, e1) =>
                {
                    bw.Dispose();
                };

                bw.ProgressChanged += (s, e1) =>
                {
                    if (object.ReferenceEquals(this.CurrentView, e1.UserState))
                    {
                        this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                    }
                };

                bw.DoWork += (s, e1) =>
                {
                    var view = this.CurrentView;
                    foreach (int progress in view.Initialize())
                    {
                        bw.ReportProgress(progress, view);
                    }
                };

                bw.RunWorkerAsync();
            }
            else
            {
                this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                this.fastListViewMain.Refresh();
                if (this.CurrentView.FirstDisplayedScrollingRowIndex.HasValue)
                    this.fastListViewMain.TopItem = this.fastListViewMain.Items[this.CurrentView.FirstDisplayedScrollingRowIndex.Value];
                if (this.CurrentView.LastCountResult.HasValue)
                    this.toolStripLabelCount.Text = this.CurrentView.LastCountResult.Value.ToString();
                if (this.CurrentView.SelectedRowIndex.HasValue)
                    this.fastListViewMain.Items[this.CurrentView.SelectedRowIndex.Value].Selected = true;
            }
        }

        private void treeViewDoc_BeforeSelect(object sender, TreeViewCancelEventArgs e)
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

        #endregion

        private void UpdateMainGridRowCount(object sender, int index)
        {
            if (object.ReferenceEquals(sender, this.CurrentView))
            {
                this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
            }
        }

        private void ChildView_ProgressChanged(object sender, ProgressItem e)
        {
            this.Invoke(new Action(() =>
            {
                if (object.ReferenceEquals(sender, this.CurrentView))
                {
                    this.progressBarMain.Value = e.Progress;
                    this.toolStripStatusLabel.Text = e.ProgressDescription;
                    this.progressBarMain.Visible = this.CurrentView.IsInProgress;
                }
            }));
        }

        #region Filter, Tag, Find, Count

        private void filterToolStripMenuItemDoc_Click(object sender, EventArgs e)
        {

        }

        private void TagCurrentView(int index, Filter filter)
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

            var childView = this.CurrentView.CreateChild(new Filter(this.toolStripTextBoxPattern.Text));
            childView.ItemAdded += this.UpdateMainGridRowCount;

            var node = this.treeViewDoc.SelectedNode.Nodes.Add(childView.Name, childView.Name);
            node.Tag = childView;
            childView.ProgressChanged += ChildView_ProgressChanged;
            this.treeViewDoc.SelectedNode = node;
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
            Filter f = new Filter(this.toolStripTextBoxPattern.Text);

            var bw = new BackgroundWorker();

            bw.RunWorkerCompleted += (s, e1) =>
            {
                var result = (Tuple<IFilteredView<DataItemBase>, int?>)e1.Result;
                if (object.ReferenceEquals(result.Item1, this.CurrentView))
                {
                    if (result.Item2.HasValue)
                    {
                        this.fastListViewMain.SelectedIndices.Clear();
                        this.fastListViewMain.Items[result.Item2.Value].Selected = true;
                        this.fastListViewMain.Items[result.Item2.Value].EnsureVisible();
                        //     this.fastListViewMain.Invalidate(this.fastListViewMain.Items[(int)e1.Result].Bounds);
                        this.toolStripLabelCount.Text = result.Item2.ToString();
                    }
                }

                bw.Dispose();
            };

            bw.DoWork += (s, e1) =>
            {
                var currentView = this.CurrentView;
                foreach (int progress in currentView.Find(f, startIndex, direction))
                {
                }

                e1.Result = Tuple.Create(currentView, currentView.SelectedRowIndex);
            };

            bw.RunWorkerAsync();
        }

        private void toolStripButtonCount_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;
            Filter f = new Filter(this.toolStripTextBoxPattern.Text);

            var bw = new BackgroundWorker();

            bw.RunWorkerCompleted += (s, e1) =>
            {
                this.toolStripLabelCount.Text = e1.Result.ToString();

                bw.Dispose();
            };

            bw.DoWork += (s, e1) =>
            {
                var currentView = this.CurrentView;
                foreach (int progress in currentView.Count(f))
                {
                }

                e1.Result = currentView.LastCountResult ?? 0;
            };

            bw.RunWorkerAsync();
        }

        #endregion

        private void fastListViewMain_CacheVirtualItems(object sender, CacheVirtualItemsEventArgs e)
        {
        }

        private void fastListViewMain_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            e.Item = new ListViewItem();
            var item = this.CurrentView.GetRowValue(e.ItemIndex);

            e.Item.Text = "";
            e.Item.Tag = item;
            e.Item.SubItems.AddRange(new List<ListViewItem.ListViewSubItem>(this.CurrentView.ColumnInfos.Count) {
                new ListViewItem.ListViewSubItem(),
                new ListViewItem.ListViewSubItem(),
                new ListViewItem.ListViewSubItem(),
                new ListViewItem.ListViewSubItem(),
                new ListViewItem.ListViewSubItem() { Tag = item == null ? null : this.CurrentView.GetColumnValue(e.ItemIndex, 4) },
                new ListViewItem.ListViewSubItem() { Tag = item == null ? null : this.CurrentView.GetColumnValue(e.ItemIndex, 5) },
                new ListViewItem.ListViewSubItem(),
            }.ToArray());
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
                var level = item.Level;
                int index = level == LogLevel.Critical ? 0 : (level == LogLevel.Error ? 1 : (level == LogLevel.Warning ? 2 : 3));

                if (index < this.fastListViewMain.LevelBrushes.Count)
                {
                    e.Graphics.FillRectangle(this.fastListViewMain.LevelBrushes[index], bound);
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

        private void fastListViewMain_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            e.DrawBackground();
            e.DrawText(TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
            var bound = e.Bounds;
            e.Graphics.DrawLine(this.fastListViewMain.GridLineColorPen, bound.X, bound.Y + bound.Height - 1, bound.X + bound.Width, bound.Y + bound.Height - 1);
        }

        private void fastListViewMain_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            Debug.WriteLine("Redrawing Row {0} Column {1}", e.ItemIndex, e.ColumnIndex);
            // draw contents.
            var bound = e.Bounds;
            DataItemBase item = (DataItemBase)e.Item.Tag;
            if (item == null) return;
            bool isSelected = e.Item.Selected;
            StringFormat format = new StringFormat(StringFormatFlags.NoWrap)
            {
                Alignment = StringAlignment.Center,
                Trimming = StringTrimming.EllipsisCharacter,
                LineAlignment = StringAlignment.Center,
            };

            Brush foreBrush = isSelected ? this.fastListViewMain.SelectionForeColorBrush : this.fastListViewMain.ForeColorBrush;

            switch (e.ColumnIndex)
            {
                case 5:
                    var paramString = (ParametricString)e.SubItem.Tag;
                    foreach (var token in paramString.GetTokens())
                    {
                        var currentFont = token.Value ? this.fastListViewMain.BoldFont : this.fastListViewMain.NormalFont;
                        e.Graphics.DrawString(
                            token.Key,
                            currentFont,
                            isSelected ? this.fastListViewMain.SelectionForeColorBrush : this.fastListViewMain.ForeColorBrush,
                            bound,
                            this.defaultStringFormat);

                        var length = e.Graphics.MeasureString(token.Key, currentFont).Width + 0.5f;
                        bound.Width -= (int)length;
                        if (bound.Width <= 0) break;
                        bound.X += (int)length;
                    }
                    break;

                case 6:
                    var colorList = (List<int>)e.SubItem.Tag;

                    var rect = e.Bounds;
                    rect.X += 4;
                    rect.Y += 4;
                    rect.Height -= 8;
                    rect.Width = rect.Height;

                    int p = 0;

                    for (int j = 0; j < this.tags.Count; j++)
                    {
                        if (p < colorList.Count && colorList[p] == j)
                        {
                            e.Graphics.FillRectangle(this.tags[j].Item2, rect);
                            p++;
                        }

                        rect.X += rect.Width + 2;
                    }
                    break;

                default:
                    e.Graphics.DrawString(item.GetColumnText(e.ColumnIndex), e.SubItem.Font, foreBrush, bound, format);
                    break;
            }
        }

        private void toolStripTextBoxPattern_TextChanged(object sender, EventArgs e)
        {
            bool enabled = !string.IsNullOrEmpty(this.toolStripTextBoxPattern.Text);
            this.toolStripButtonFilter.Enabled = enabled;
            this.toolStripButtonCount.Enabled = enabled;
            this.toolStripSplitButtonFind.Enabled = enabled;
            this.toolStripSplitButtonTag.Enabled = enabled;
        }

        private void fastListViewMain_Resize(object sender, EventArgs e)
        {
            var textWidth = this.fastListViewMain.Width;
            ColumnHeader textCol = null;

            foreach (ColumnHeader col in this.fastListViewMain.Columns)
            {
                if (string.Equals(col.Name, "Text"))
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
    }
}
