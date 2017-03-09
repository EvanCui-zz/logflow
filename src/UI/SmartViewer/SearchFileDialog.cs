namespace LogFlow.Viewer
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Windows.Forms;
    using LogFlow.DataModel;

    public partial class SearchFileDialog : Form
    {
        private int currentFinished;
        private int totalCount;
        private CancellationTokenSource cts;

        public SearchFileDialog()
        {
            InitializeComponent();
        }

        private void SearchFileDialog_Load(object sender, EventArgs e)
        {
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            if (this.folderBrowserDialog.ShowDialog() == DialogResult.OK)
            {
                this.comboBoxSearchFolder.Text = this.folderBrowserDialog.SelectedPath;
            }
        }

        public IList<string> SelectedFilePaths => (from DataGridViewRow row in this.dataGridViewResult.SelectedRows select (string)row.Tag).ToList();

        public IFilter Filter { get; private set; }

        public IList<string> MatchedFilePaths => (from DataGridViewRow row in this.dataGridViewResult.Rows where ((string)row.Cells[1].Value).Length > 0 select (string)row.Tag).ToList();

        private async void buttonSearch_Click(object sender, EventArgs e)
        {
            this.cts?.Cancel();
            this.cts?.Dispose();
            this.EnableButtons(false);

            this.currentFinished = 0;
            this.progressBar1.Value = 0;

            this.cts = new CancellationTokenSource();
            this.Filter = new Filter(this.comboBoxSearchPattern.Text);

            var filePaths = Directory.EnumerateFiles(
                this.comboBoxSearchFolder.Text,
                this.comboBoxFileNamePattern.Text,
                this.checkBoxRecursive.Checked ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly).ToList();

            this.totalCount = filePaths.Count;

            this.dataGridViewResult.Rows.Clear();
            this.dataGridViewResult.Rows.AddRange(filePaths.Select(p => new DataGridViewRow()
            {
                Cells =
                {
                    new DataGridViewTextBoxCell() {Value = Path.GetFileName(p)},
                    new DataGridViewTextBoxCell() {Value = string.Empty},
                    new DataGridViewTextBoxCell() {Value = "0 %"},
                },
                Tag = p,
            }).ToArray());

            int lineCount;
            int.TryParse(this.comboBoxLineCount.Text, out lineCount);

            await Task.WhenAll(Enumerable.Range(0, filePaths.Count).Select(i => Task.Run(() =>
            {
                DataItemBase dataItem;
                var logSource = LogSourceManager.Instance.GetLogSource(filePaths[i]);
                logSource.ItemAdded += (o, i1) =>
                {
                    dataItem = logSource.Items[i1];
                    this.dataGridViewResult.Rows[i].Cells[1].Value =
                        string.Format(logSource.Templates[dataItem.TemplateId], dataItem.Parameters);
                };

                foreach (var progress in logSource.Peek(this.Filter, lineCount, cts.Token))
                {
                    this.dataGridViewResult.Rows[i].Cells[2].Value = $"{progress} %";
                }

                this.dataGridViewResult.Rows[i].Cells[2].Value = "100 %";

                this.currentFinished++;
                (logSource as IDisposable)?.Dispose();

                this.progressBar1.Invoke(new Action(() => this.progressBar1.Value = 100 * this.currentFinished / this.totalCount));
            })));

            this.EnableButtons(true);
        }

        private void EnableButtons(bool enabled)
        {
            this.buttonOpenMatched.Enabled = this.buttonOpenSelected.Enabled = enabled;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.cts?.Cancel();
            this.cts?.Dispose();
            this.cts = null;
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void buttonOpenSelected_Click(object sender, EventArgs e)
        {
            this.cts?.Cancel();
            this.cts?.Dispose();
            this.cts = null;
            this.DialogResult = DialogResult.Yes;
            this.Close();
        }

        private void buttonOpenMatched_Click(object sender, EventArgs e)
        {
            this.cts?.Cancel();
            this.cts?.Dispose();
            this.cts = null;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void dataGridViewResult_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            this.buttonOpenSelected_Click(sender, e);
        }

        private void buttonOpenFiltered_Click(object sender, EventArgs e)
        {
            this.cts?.Cancel();
            this.cts?.Dispose();
            this.cts = null;
            this.DialogResult = DialogResult.Retry;
            this.Close();
        }

        private void comboBoxLineCount_Validating(object sender, System.ComponentModel.CancelEventArgs e)
        {
            int lineCount;
            if (true == (e.Cancel = !int.TryParse(this.comboBoxLineCount.Text, out lineCount)))
            {
                this.errorProvider1.SetError(this.comboBoxLineCount, "An integer is required.");
            }
            else
            {
                this.errorProvider1.Clear();
            }
        }
    }
}
