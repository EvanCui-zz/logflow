using LogFlow.DataModel;
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

namespace LogFlow.Viewer
{
    public partial class MainForm : Form
    {
        private RootView<DataItemBase> document;
        private Font boldFont;
        private Font normalFont;
        private SolidBrush foreColorBrush;
        private SolidBrush selectionForeColorBrush;
        private StringFormat defaultStringFormat;

        private List<Brush> levelBrushes = new List<Brush>()
        {
            new SolidBrush(Color.FromArgb(255, 0, 0)),
            new SolidBrush(Color.FromArgb(200, 0, 0)),
            new SolidBrush(Color.FromArgb(200, 180, 0)),
        };

        private List<Tuple<Color, SolidBrush, Pen>> tags = new List<Tuple<Color, SolidBrush, Pen>>()
        {
            new Tuple<Color, SolidBrush, Pen>(Color.Cyan, new SolidBrush(Color.Cyan), new Pen(Color.Cyan)),
            new Tuple<Color, SolidBrush, Pen>(Color.FromArgb(128, 128, 255), new SolidBrush(Color.FromArgb(128, 128, 255)), new Pen(Color.FromArgb(128, 128, 255))),
            new Tuple<Color, SolidBrush, Pen>(Color.FromArgb(128, 255, 128), new SolidBrush(Color.FromArgb(128, 255, 128)), new Pen(Color.FromArgb(128, 255, 128))),
        };

        public MainForm()
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
            Debug.WriteLine("RowPrePaint {0}", e.RowIndex);
          //  return;

            var level = (LogLevels)this.CurrentView.GetColumnValue(e.RowIndex, 3);
            int index = level == LogLevels.Critical ? 0 : (level == LogLevels.Error ? 1 : (level == LogLevels.Warning ? 2 : 3));

            if (index < this.levelBrushes.Count)
            {
                e.Graphics.FillRectangle(this.levelBrushes[index], e.RowBounds);
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
            this.document = new RootView<DataItemBase>(LogSourceManager.Instance.GetLogSource("loaded test"));
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

            this.dataGridViewMain.Columns.Clear();
            this.dataGridViewMain.Rows.Clear();

            this.dataGridViewMain.Columns.AddRange(columns);
            this.document.ItemAdded += this.UpdateMainGridRowCount;
            this.treeViewDoc.SelectedNode = node;
        }

        public FilteredView<DataItemBase> CurrentView { get; set; }

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
                    bw.Dispose();
                };

                bw.ProgressChanged += (s, e1) =>
                {
                    if (this.progressBarMain.Value != e1.ProgressPercentage)
                    {
                        this.progressBarMain.Value = e1.ProgressPercentage;
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
                this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
            }

            Debug.WriteLine("Setting scroll index");
            if (this.CurrentView.FirstDisplayedScrollingRowIndex.HasValue)
                this.dataGridViewMain.FirstDisplayedScrollingRowIndex = this.CurrentView.FirstDisplayedScrollingRowIndex.Value;
            // Add rows to DGV here
          //  SendMessage(this.dataGridViewMain.Handle, WM_SETREDRAW, true, 0);
            this.dataGridViewMain.Refresh();
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
            var f = new Filter(this.toolStripTextBoxPattern.Text);

            var bw = new BackgroundWorker();
            bw.WorkerReportsProgress = true;
            this.progressBarMain.Visible = true;
            this.progressBarMain.Value = 0;

            bw.RunWorkerCompleted += (s, e1) =>
            {
                int selection = (int)e1.Result;
                if (selection != -1)
                {
                    this.dataGridViewMain.ClearSelection();
                    this.dataGridViewMain.CurrentCell = this.dataGridViewMain[0, (int)e1.Result];
                }
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
                var currentView = this.CurrentView;
                foreach (int progress in currentView.Find(f, startIndex, direction))
                {
                    bw.ReportProgress(progress);
                }

                e1.Result = currentView.SelectedRowIndex ?? -1;
            };

            bw.RunWorkerAsync();
        }

        private void toolStripButtonCount_Click(object sender, EventArgs e)
        {
            if (this.CurrentView == null) return;
            IFilter f = new Filter(this.toolStripTextBoxPattern.Text);

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
                var currentView = this.CurrentView;
                foreach (int progress in currentView.Count(f))
                {
                    bw.ReportProgress(progress);
                }

                e1.Result = currentView.LastCountResult ?? 0;
            };

            bw.RunWorkerAsync();
        }

        private void treeViewDoc_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (this.CurrentView == null) return;
            this.CurrentView.FirstDisplayedScrollingRowIndex = this.dataGridViewMain.FirstDisplayedScrollingRowIndex;
        }
    }
}
