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

            for (int i = 0; i < 10000; i ++)
            {
                this.source.Templates.Add(string.Format("string templates {{0}}, {{1}}, testing long string, {0}", i));
            }

            this.source.Items = new List<DataItemBase>();

            for (int i = 0; i< 10000000; i++)
            {
                this.source.Items.Add(new DataItemBase()
                {
                    Id = i,
                    ThreadId = i % 100,
                    Time = DateTime.UtcNow.AddSeconds(i),
                    TemplateId = r.Next(10000),
                    Parameters = new object[] { DateTime.UtcNow, i + 255 },
                    ProcessId = i / 100000,
                });
            }

            this.dataGridViewMain.Columns.AddRange(this.source.ColumnInfos.Select(ci => new DataGridViewTextBoxColumn()
            {
                Name = ci.Name,
                MinimumWidth = ci.MinWidth,
                Width = ci.Width,
                HeaderText = ci.Name,
            }).ToArray());

            this.dataGridViewMain.RowCount = this.source.Items.Count;
        }
    }
}
