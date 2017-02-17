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

namespace SmartViewer
{
    public partial class MainForm : Form
    {
        private LogDocument<DataItemBase> document;
        private List<Color> tags = new List<Color>()
        {
            Color.Cyan,
            Color.FromArgb(128, 128, 255),
            Color.FromArgb(128, 255, 128),
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
            Debug.WriteLine("cell value needed row {0}, col {1}", e.RowIndex, e.ColumnIndex);
            e.Value = this.CurrentView.GetColumnValue(e.RowIndex, e.ColumnIndex);
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            int i = 0;
            this.toolStripSplitButtonTag.DropDownItems.AddRange(this.tags.Select(t => new ToolStripMenuItem($"Tag {++i}", null, (s, e1) =>
            {
                if (this.CurrentView == null) return;
                var currentMenuItem = (ToolStripMenuItem)s;
                bool tag = !string.IsNullOrEmpty(this.toolStripTextBoxPattern.Text);
                currentMenuItem.Checked = tag;
                this.TagCurrentView(t, tag ? new Filter(this.toolStripTextBoxPattern.Text) : null);
            })
            {
                BackColor = t,
            }).ToArray());

            this.toolStripSplitButtonTag.DefaultItem = this.toolStripSplitButtonTag.DropDownItems[0];
            this.toolStripSplitButtonFind.DefaultItem = this.findNextToolStripMenuItem;
        }

        private void dataGridViewMain_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            var row = this.dataGridViewMain.Rows[e.RowIndex];
            Debug.WriteLine("row pre paint row {0}", e.RowIndex);

            var level = (LogLevel)this.CurrentView.GetColumnValue(e.RowIndex, 3);
            if (level == LogLevel.Warning)
            {
                row.DefaultCellStyle.BackColor = Color.FromArgb(200, 180, 0);
            }
            else if (level == LogLevel.Error)
            {
                row.DefaultCellStyle.BackColor = Color.FromArgb(200, 0, 0);
            }
            else if (level == LogLevel.Error)
            {
                row.DefaultCellStyle.BackColor = Color.FromArgb(255, 0, 0);
            }
        }

        private void dataGridViewMain_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.RowIndex == -1) return;

            e.PaintBackground(e.CellBounds, true);
            var bound = e.CellBounds;

            if (e.ColumnIndex == 4)
            {
                e.Handled = true;
                var paramString = e.Value as ParametricString;
                if (paramString == null) return;

                var boldFont = new Font(e.CellStyle.Font, FontStyle.Bold);

                foreach (var token in paramString.GetTokens())
                {
                    var currentFont = token.Value ? boldFont : e.CellStyle.Font;
                    e.Graphics.DrawString(token.Key, currentFont, new SolidBrush((e.State & DataGridViewElementStates.Selected) == DataGridViewElementStates.Selected ? e.CellStyle.SelectionForeColor : e.CellStyle.ForeColor), bound, new StringFormat(StringFormatFlags.NoWrap)
                    {
                        Trimming = StringTrimming.EllipsisCharacter,
                        LineAlignment = StringAlignment.Center,
                    });

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
                var colorList = e.Value as List<Color>;
                if (colorList == null) return;

                var rect = e.CellBounds;
                rect.X += 4;
                rect.Y += 4;
                rect.Height -= 8;
                rect.Width = rect.Height;

                foreach (var c in colorList)
                {
                    //                    e.Graphics.DrawRectangle(new Pen(c), rect);
                    e.Graphics.FillRectangle(new SolidBrush(c), rect);

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
                AutoSizeMode = string.Equals(ci.Name, "Text") ? DataGridViewAutoSizeColumnMode.Fill : (string.Equals(ci.Name, "Tag") ? DataGridViewAutoSizeColumnMode.None : DataGridViewAutoSizeColumnMode.DisplayedCells),
                MinimumWidth = 60,
                Width = 60,
            }).ToArray();

            this.dataGridViewMain.Columns.Clear();
            this.dataGridViewMain.Rows.Clear();

            this.dataGridViewMain.Columns.AddRange(columns);
            this.document.ItemAdded += this.UpdateMainGridRowCount;
            this.document.TestGenerateFakeData();
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

            this.dataGridViewMain.Rows.Clear();

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

            if (this.CurrentView.FirstDisplayedScrollingRowIndex.HasValue)
                this.dataGridViewMain.FirstDisplayedScrollingRowIndex = this.CurrentView.FirstDisplayedScrollingRowIndex.Value;
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

        private void TagCurrentView(Color color, Filter filter)
        {
            if (this.CurrentView == null) return;

            if (filter == null)
            {
                this.CurrentView.Tags.Remove(color);
            }
            else
            {
                this.CurrentView.Tags[color] = filter;
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

        private void Find(int startIndex, bool nextDirection)
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
                this.dataGridViewMain.CurrentCell = this.dataGridViewMain[0, ((KeyValuePair<int, bool>)e1.Result).Key];
                this.toolStripLabelCount.Text = ((KeyValuePair<int, bool>)e1.Result).Key.ToString();
                this.toolStripStatusLabel.Text = "Ready";
                this.progressBarMain.Visible = false;

                bw.Dispose();
            };

            bw.ProgressChanged += (s, e1) =>
            {
                if (this.progressBarMain.Value != e1.ProgressPercentage)
                {
                    this.progressBarMain.Value = e1.ProgressPercentage;
                    this.toolStripStatusLabel.Text = $"Searching ... {e1.ProgressPercentage}%";
                }
            };

            bw.DoWork += (s, e1) =>
            {
                ResultWrapper<KeyValuePair<int, bool>> findResult = new ResultWrapper<KeyValuePair<int, bool>>();
                foreach (int progress in this.CurrentView.Find(f, startIndex, findResult, nextDirection))
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
    }
}
