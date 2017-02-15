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
    public partial class Form1 : Form
    {
        DataSource<DataItemBase> source = new DataSource<DataItemBase>();

        public Form1()
        {
            InitializeComponent();
        }

        private void dataGridViewMain_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridViewMain_CellValueNeeded(object sender, DataGridViewCellValueEventArgs e)
        {
            e.Value = source.GetColumnValue(e.RowIndex, e.ColumnIndex);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Random r = new Random();

            this.source.Templates = new List<string>();

            for (int i = 0; i < 10000; i++)
            {
                this.source.Templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, {0}", i));
            }

            this.source.Items = new List<DataItemBase>();

            for (int i = 0; i < 100000; i++)
            {
                this.source.Items.Add(new DataItemBase()
                {
                    Id = i,
                    ThreadId = i % 100,
                    Time = DateTime.UtcNow.AddSeconds(i),
                    TemplateId = r.Next(10000),
                    Parameters = new object[] { DateTime.UtcNow, i + 255 },
                    ProcessId = i / 100000,
                    Level = (LogLevel)r.Next((int)LogLevel.Verbose) + 1,
                });
            }

            var columns = this.source.ColumnInfos.Select(ci => new DataGridViewTextBoxColumn()
            {
                Name = ci.Name,
                HeaderText = ci.Name,
                AutoSizeMode = string.Equals(ci.Name, "Text") ? DataGridViewAutoSizeColumnMode.Fill : DataGridViewAutoSizeColumnMode.DisplayedCells,
            }).ToArray();

            this.dataGridViewMain.Columns.AddRange(columns);

            this.dataGridViewMain.RowCount = this.source.Items.Count;
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
            if (e.ColumnIndex == 4)
            {
                e.PaintBackground(e.CellBounds, true);
                string text = e.Value as string;

                var bound = e.CellBounds;
                bool isBold = false;
                int p = 0;

                var boldFont = new Font(e.CellStyle.Font, FontStyle.Bold);
                while (text.Length > 0)
                {
                    int l = text.IndexOf("{{{", p);
                    if (l == -1) l = text.Length;
                    int r = text.IndexOf("}}}", p);
                    if (r == -1) r = text.Length;

                    int next = Math.Min(l, r);
                    if (next > 0)
                    {
                        string currentToken = text.Substring(p, next - p);

                        var currentFont = isBold ? boldFont : e.CellStyle.Font;
                        e.Graphics.DrawString(currentToken, currentFont, Brushes.DeepPink, bound, new StringFormat(StringFormatFlags.NoWrap)
                        {
                            Trimming = StringTrimming.EllipsisCharacter,
                            LineAlignment = StringAlignment.Center,
                        });
                        
                        var length = e.Graphics.MeasureString(currentToken, currentFont).Width;
                        bound.X += (int)length;
                        bound.Width -= (int)length;
                        if (bound.Width <= 0) break;
                    }

                    if (next == text.Length) break;

                    isBold = l < r;
                    p = next + "{{{".Length;
                }

                e.Handled = true;
            }
        }
    }
}
