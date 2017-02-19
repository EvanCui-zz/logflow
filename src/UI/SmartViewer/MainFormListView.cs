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
        private Font boldFont;
        private Font normalFont;
        private SolidBrush foreColorBrush;
        private SolidBrush selectionForeColorBrush;
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

        private void dataGridViewMain_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridViewMain_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            //    Debug.WriteLine("cell value needed row {0}, col {1}", e.RowIndex, e.ColumnIndex);
            e.Value = this.CurrentView.GetColumnValue(e.RowIndex, e.ColumnIndex);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            var pi = this.dataGridViewMain.GetType().GetProperty("DoubleBuffered", System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic);
            pi.SetValue(this.dataGridViewMain, true);

            this.fastListViewMain.SelectionForeColorBrush = new SolidBrush(this.fastListViewMain.SelectionForeColor);
            this.fastListViewMain.SelectionBackColorBrush = new SolidBrush(this.fastListViewMain.SelectionBackColor);
            this.fastListViewMain.AlternateBackColorBrush = new SolidBrush(this.fastListViewMain.AlternateBackColor);
            this.fastListViewMain.GridLineColorPen = new Pen(this.fastListViewMain.GridLineColor);
            this.fastListViewMain.SetHeight(21);

            this.boldFont = new Font(this.dataGridViewMain.Font, FontStyle.Bold);
            this.normalFont = this.dataGridViewMain.Font;

            this.foreColorBrush = new SolidBrush(this.dataGridViewMain.DefaultCellStyle.ForeColor);
            this.selectionForeColorBrush = new SolidBrush(this.dataGridViewMain.DefaultCellStyle.SelectionForeColor);

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
        }

        private void dataGridViewMain_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            if (e.State.HasFlag(DataGridViewElementStates.Selected)) return;

            Debug.WriteLine("RowPrePaint {0}", e.RowIndex);
            //  return;

            var level = (LogLevel)this.CurrentView.GetColumnValue(e.RowIndex, 3);
            int index = level == LogLevel.Critical ? 0 : (level == LogLevel.Error ? 1 : (level == LogLevel.Warning ? 2 : 3));

            if (index < this.fastListViewMain.LevelBrushes.Count)
            {
                e.Graphics.FillRectangle(this.fastListViewMain.LevelBrushes[index], e.RowBounds);
                e.PaintParts &= ~DataGridViewPaintParts.Background;
                e.PaintParts &= ~DataGridViewPaintParts.ContentBackground;
            }
        }

        private void dataGridViewMain_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.RowIndex == -1) return;

            var bound = e.CellBounds;

            if (e.ColumnIndex == 4)
            {
                e.Handled = true;
                if (e.Value == null) return;

                bool isSelected = (e.State & DataGridViewElementStates.Selected) == DataGridViewElementStates.Selected;
                e.Paint(e.CellBounds, e.PaintParts & ~DataGridViewPaintParts.ContentForeground & ~DataGridViewPaintParts.ErrorIcon);

                var paramString = (ParametricString)e.Value;

                foreach (var token in paramString.GetTokens())
                {
                    var currentFont = token.Value ? this.boldFont : this.normalFont;
                    e.Graphics.DrawString(
                        token.Key,
                        currentFont,
                        isSelected ? this.selectionForeColorBrush : this.foreColorBrush,
                        bound,
                        this.defaultStringFormat);

                    var length = e.Graphics.MeasureString(token.Key, currentFont).Width + 0.5f;
                    bound.Width -= (int)length;
                    if (bound.Width <= 0) break;
                    bound.X += (int)length;
                }

                e.Handled = true;
            }
            else if (e.ColumnIndex == 5)
            {
                e.Handled = true;
                if (e.Value == null) return;

                var colorList = (List<int>)e.Value;

                e.Paint(e.CellBounds, e.PaintParts & ~DataGridViewPaintParts.ContentForeground & ~DataGridViewPaintParts.ErrorIcon);

                var rect = e.CellBounds;
                rect.X += 4;
                rect.Y += 4;
                rect.Height -= 8;
                rect.Width = rect.Height;

                int p = 0;

                for (int i = 0; i < this.tags.Count; i++)
                {
                    if (p < colorList.Count && colorList[p] == i)
                    {
                        e.Graphics.FillRectangle(this.tags[i].Item2, rect);
                        p++;
                    }

                    rect.X += rect.Width + 2;
                }
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.document = new LogDocument<DataItemBase>("loaded test");
            this.treeViewDoc.Nodes.Clear();
            var node = this.treeViewDoc.Nodes.Add("Root", this.document.Name);
            node.Tag = this.document;

            var columns = this.document.ColumnInfos.Select(ci => new DataGridViewTextBoxColumn()
            {
                Name = ci.Name,
                HeaderText = ci.Name,
                AutoSizeMode = string.Equals(ci.Name, "Text") ? DataGridViewAutoSizeColumnMode.Fill : DataGridViewAutoSizeColumnMode.None,
                MinimumWidth = 5,
                Width = ci.Width,
            }).ToArray();

            var headers = this.document.ColumnInfos.Select(ci => new ColumnHeader()
            {
                Name = ci.Name,
                Text = ci.Name,
                Width = ci.Width,
            }).ToArray();


            this.dataGridViewMain.Columns.Clear();
            this.dataGridViewMain.Rows.Clear();

            this.dataGridViewMain.Columns.AddRange(columns);

            this.fastListViewMain.Columns.Clear();
            this.fastListViewMain.Items.Clear();
            this.fastListViewMain.Columns.AddRange(headers);
            this.document.ItemAdded += this.UpdateMainGridRowCount;
            this.document.GenerateFakeData();
            this.treeViewDoc.SelectedNode = node;
        }

        public FilteredView<DataItemBase> CurrentView { get; set; }
        [DllImport("user32.dll")]
        private static extern int SendMessage(IntPtr hWnd, Int32 wMsg, bool wParam, Int32 lParam);
        private const int WM_SETREDRAW = 11;



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
            this.dataGridViewMain.Rows.Clear();

            Debug.WriteLine("rows cleared");
            if (!this.CurrentView.IsInitialized)
            {
                this.progressBarMain.Value = 0;
                this.progressBarMain.Visible = true;
                var bw = new BackgroundWorker();
                bw.WorkerReportsProgress = true;

                bw.RunWorkerCompleted += (s, e1) =>
                {
                    this.progressBarMain.Visible = false;
                    this.toolStripStatusLabel.Text = "Ready";
                    this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
                    this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                    bw.Dispose();
                };

                bw.ProgressChanged += (s, e1) =>
                {
                    if (this.progressBarMain.Value != e1.ProgressPercentage)
                    {
                        this.progressBarMain.Value = e1.ProgressPercentage;
                        this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                        this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
                        this.toolStripStatusLabel.Text = $"Filtering ... {e1.ProgressPercentage}%";
                    }
                };

                bw.DoWork += (s, e1) =>
                {
                    foreach (int progress in this.CurrentView.Initialize())
                    {
                        bw.ReportProgress(progress);
                    }
                };

                bw.RunWorkerAsync();
                this.toolStripStatusLabel.Text = "Filtering ...";
            }
            else
            {
                this.fastListViewMain.VirtualListSize = this.CurrentView.TotalCount;
                this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
            }

            Debug.WriteLine("Setting scroll index");

            if (this.CurrentView.FirstDisplayedScrollingRowIndex.HasValue)
                this.dataGridViewMain.FirstDisplayedScrollingRowIndex = this.CurrentView.FirstDisplayedScrollingRowIndex.Value;
            if (this.CurrentView.AutoScrollOffset.HasValue)
                this.fastListViewMain.AutoScrollOffset = this.CurrentView.AutoScrollOffset.Value;
            // Add rows to DGV here
            //  SendMessage(this.dataGridViewMain.Handle, WM_SETREDRAW, true, 0);
            this.dataGridViewMain.Refresh();
            this.fastListViewMain.Refresh();
            Debug.WriteLine("Resume layout");
        }

        private void UpdateMainGridRowCount(object sender, int index)
        {
            if (object.ReferenceEquals(sender, this.CurrentView))
            {
                this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
            }
        }

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

            this.dataGridViewMain.Refresh();
        }

        private void toolStripButtonFilter_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;

            var childView = this.CurrentView.CreateChild(new Filter(this.toolStripTextBoxPattern.Text));
            childView.ItemAdded += this.UpdateMainGridRowCount;

            var node = this.treeViewDoc.SelectedNode.Nodes.Add(childView.Name, childView.Name);
            node.Tag = childView;
            this.treeViewDoc.SelectedNode = node;
        }

        private void toolStripSplitButtonTag_ButtonClick(object sender, EventArgs e)
        {

        }

        private void findPreviousToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int currentIndex = this.dataGridViewMain.RowCount - 1;
            if (this.dataGridViewMain.SelectedRows.Count > 0)
            {
                currentIndex = this.dataGridViewMain.SelectedRows[this.dataGridViewMain.SelectedRows.Count - 1].Index;
            }

            this.Find(currentIndex, false);
        }

        private void findNextToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int currentIndex = 0;
            if (this.dataGridViewMain.SelectedRows.Count > 0)
            {
                currentIndex = this.dataGridViewMain.SelectedRows[0].Index;
            }

            this.Find(currentIndex, true);
        }

        private void Find(int startIndex, bool direction)
        {
            if (this.CurrentView == null) return;
            Filter f = new Filter(this.toolStripTextBoxPattern.Text);

            var bw = new BackgroundWorker();
            bw.WorkerReportsProgress = true;
            this.progressBarMain.Visible = true;
            this.progressBarMain.Value = 0;

            bw.RunWorkerCompleted += (s, e1) =>
            {
                this.dataGridViewMain.ClearSelection();
                this.dataGridViewMain.CurrentCell = this.dataGridViewMain[0, (int)e1.Result];
                this.toolStripLabelCount.Text = e1.Result.ToString();
                this.toolStripStatusLabel.Text = "Ready";
                this.progressBarMain.Visible = false;

                bw.Dispose();
            };

            bw.ProgressChanged += (s, e1) =>
            {
                if (this.progressBarMain.Value < e1.ProgressPercentage)
                {
                    this.progressBarMain.Value = e1.ProgressPercentage;
                    this.toolStripStatusLabel.Text = $"Searching ... {e1.ProgressPercentage}%";
                }
            };

            bw.DoWork += (s, e1) =>
            {
                var findResult = new ResultWrapper<int>();
                foreach (int progress in this.CurrentView.Find(f, startIndex, direction, findResult))
                {
                    bw.ReportProgress(progress);
                }

                e1.Result = findResult.Result;
            };

            bw.RunWorkerAsync();
        }

        private void toolStripButtonCount_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;
            Filter f = new Filter(this.toolStripTextBoxPattern.Text);

            var bw = new BackgroundWorker();
            bw.WorkerReportsProgress = true;
            this.progressBarMain.Visible = true;
            this.progressBarMain.Value = 0;

            bw.RunWorkerCompleted += (s, e1) =>
            {
                this.toolStripLabelCount.Text = e1.Result.ToString();
                this.toolStripStatusLabel.Text = "Ready";
                this.progressBarMain.Visible = false;

                bw.Dispose();
            };

            bw.ProgressChanged += (s, e1) =>
            {
                if (this.progressBarMain.Value != e1.ProgressPercentage)
                {
                    this.progressBarMain.Value = e1.ProgressPercentage;
                    this.toolStripStatusLabel.Text = $"Counting ... {e1.ProgressPercentage}%";
                }
            };

            bw.DoWork += (s, e1) =>
            {
                ResultWrapper<int> countResult = new ResultWrapper<int>();
                foreach (int progress in this.CurrentView.Count(f, countResult))
                {
                    bw.ReportProgress(progress);
                }

                e1.Result = countResult.Result;
            };

            bw.RunWorkerAsync();
        }

        private void treeViewDoc_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (this.CurrentView == null) return;
            this.CurrentView.FirstDisplayedScrollingRowIndex = this.dataGridViewMain.FirstDisplayedScrollingRowIndex;
        }

        private void fastListViewMain_CacheVirtualItems(object sender, CacheVirtualItemsEventArgs e)
        {
        }

        private void fastListViewMain_RetrieveVirtualItem(object sender, RetrieveVirtualItemEventArgs e)
        {
            e.Item = new ListViewItem();
            var item = this.CurrentView.GetRowValue(e.ItemIndex);
            e.Item.Text = item.Id.ToString();
            e.Item.Tag = item;
            e.Item.SubItems.AddRange(new List<ListViewItem.ListViewSubItem>(this.CurrentView.ColumnInfos.Count)
            {
                new ListViewItem.ListViewSubItem() { Text = item.Time.ToString() },
                new ListViewItem.ListViewSubItem() { Text = item.ThreadId.ToString() },
                new ListViewItem.ListViewSubItem() { Text = item.Level.ToString() },
                new ListViewItem.ListViewSubItem() { Tag = this.CurrentView.GetColumnValue(e.ItemIndex, 4) },
                new ListViewItem.ListViewSubItem() { Tag = this.CurrentView.GetColumnValue(e.ItemIndex, 5) },
                new ListViewItem.ListViewSubItem() { Text = item.ProcessId.ToString() },
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
                case 4:
                    var paramString = (ParametricString)e.SubItem.Tag;
                    foreach (var token in paramString.GetTokens())
                    {
                        var currentFont = token.Value ? this.boldFont : this.normalFont;
                        e.Graphics.DrawString(
                            token.Key,
                            currentFont,
                            isSelected ? this.selectionForeColorBrush : this.foreColorBrush,
                            bound,
                            this.defaultStringFormat);

                        var length = e.Graphics.MeasureString(token.Key, currentFont).Width + 0.5f;
                        bound.Width -= (int)length;
                        if (bound.Width <= 0) break;
                        bound.X += (int)length;
                    }
                    break;

                case 5:
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
                    e.Graphics.DrawString(e.SubItem.Text, e.SubItem.Font, foreBrush, bound, format);
                    break;
            }
        }

        private void fastListViewMain_MouseMove(object sender, MouseEventArgs e)
        {
            var item = this.fastListViewMain.GetItemAt(e.X, e.Y);
            if (item != null && !this.CurrentView.HasHacked[item.Index])
            {
                Debug.WriteLine("Mouse move hack {0}", item.Index);
                this.CurrentView.HasHacked[item.Index] = true;
                this.fastListViewMain.Invalidate(item.Bounds);
            }
        }
    }
}
