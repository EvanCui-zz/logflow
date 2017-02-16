using DataModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
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

        public MainForm()
        {
            InitializeComponent();
        }

        private void dataGridViewMain_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridViewMain_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            e.Value = this.CurrentView.GetColumnValue(e.RowIndex, e.ColumnIndex);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        private void dataGridViewMain_RowPostPaint(object sender, DataGridViewRowPostPaintEventArgs e)
        {
            e.PaintCellsBackground(e.ClipBounds, true);
            e.PaintCellsContent(e.ClipBounds);
        }

        private void dataGridViewMain_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            var row = this.dataGridViewMain.Rows[e.RowIndex];
            if (((LogLevel)row.Cells[3].Value) == LogLevel.Warning)
            {
                row.DefaultCellStyle.BackColor = Color.Yellow;
            }
        }

        private void dataGridViewMain_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.RowIndex == -1) return;

            if (e.ColumnIndex == 4)
            {
                e.PaintBackground(e.CellBounds, true);
                var paramString = e.Value as ParametricString;

                var bound = e.CellBounds;
                var boldFont = new Font(e.CellStyle.Font, FontStyle.Bold);

                foreach (var token in paramString.GetTokens())
                {
                    var currentFont = token.Value ? boldFont : e.CellStyle.Font;
                    e.Graphics.DrawString(token.Key, currentFont, new SolidBrush(e.CellStyle.ForeColor), bound, new StringFormat(StringFormatFlags.NoWrap)
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
                AutoSizeMode = string.Equals(ci.Name, "Text") ? DataGridViewAutoSizeColumnMode.Fill : DataGridViewAutoSizeColumnMode.DisplayedCells,
            }).ToArray();

            this.dataGridViewMain.Columns.Clear();

            this.dataGridViewMain.Columns.AddRange(columns);
            this.document.ItemAdded += (s, index) =>
            { //if (this.CurrentView.TotalCount % 100 == 0) this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;

                //       this.dataGridViewMain.Refresh();
            };
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

            this.dataGridViewMain.RowCount = this.CurrentView.TotalCount;
            this.dataGridViewMain.Refresh();
        }

        private void filterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            BackgroundWorker bw = new BackgroundWorker();
            bw.RunWorkerCompleted += (s, e1) =>
            {
                bw.Dispose();
            };

            bw.DoWork += (s, e1) =>
            {
                Filter f = new Filter(this.toolStripTextBox1.Text);

                Action a = () => this.toolStripStatusLabel.Text = "Filtering ...";
                this.Invoke(a);

                ResultWrapper<FilteredView<DataItemBase>> result = new ResultWrapper<FilteredView<DataItemBase>>();
                foreach (int progress in this.CurrentView.CreateChild(f, result))
                {
                    a = () =>
                    {
                        this.toolStripProgressBarStatus.Value = progress;
                        this.toolStripStatusLabel.Text = $"Filtering ... {progress}%";
                    };

                    this.Invoke(a);
                }

                a = () =>
                {
                    var node = this.treeViewDoc.SelectedNode.Nodes.Add(result.Result.Name, result.Result.Name);
                    node.Tag = result.Result;
                    this.treeViewDoc.SelectedNode = node;
                    this.toolStripStatusLabel.Text = "Ready";
                };

                this.Invoke(a);
            };

            bw.RunWorkerAsync();
        }

        private void toolStripSplitButton_ButtonClick(object sender, EventArgs e)
        {
            //  if (this.toolsToolStripMenuItem.)
        }

        private void filterToolStripMenuItemDoc_Click(object sender, EventArgs e)
        {

        }

        private void highlightToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
    }
}
