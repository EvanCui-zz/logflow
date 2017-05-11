namespace LogFlow.Viewer
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Linq;
    using System.Windows.Forms;
    using LogFlow.DataModel;
    using LogFlow.Viewer.Properties;

    public partial class AllSettings : Form
    {
        public AllSettings()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.ApplyChanges();
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Settings.Default.Reload();
            this.Close();
        }

        private void AllSettings_Load(object sender, EventArgs e)
        {
            // first page:
            this.checkBoxColorLevel.Checked = Settings.Default.Display_ColoredLevel;
            if (Settings.Default.Display_LevelColors == null) Settings.Default.Display_LevelColors = new LevelColors();
            this.propertyGridLevelColor.SelectedObject = Settings.Default.Display_LevelColors.Clone();

            if (Settings.Default.Display_TagColors == null) Settings.Default.Display_TagColors = new TagColors();
            this.propertyGridTagColors.SelectedObject = Settings.Default.Display_TagColors.Clone();

            this.checkBoxBoldParameter.Checked = Settings.Default.Display_BoldParameter;
            this.checkBoxHighlightMatch.Checked = Settings.Default.Display_HighlightFind;
            this.checkBoxFastRendering.Checked = Settings.Default.Display_FastRendering;
            this.textBoxIndentSize.Text = Settings.Default.Indent_Size.ToString();

            // second page:
            this.checkBoxAutoLoad.Checked = Settings.Default.Behavior_AutoLoad;
            this.checkBoxAutoScroll.Checked = Settings.Default.Behavior_AutoScroll;
            this.checkBoxDataVirtual.Checked = Settings.Default.Behavior_DataVirtualization;
            this.checkBoxInternStrings.Checked = Settings.Default.Behavior_BackgroundInternStrings;
            this.textBoxInternInterval.Text = Settings.Default.Behavior_InternIntervalMilliseconds.ToString();
            this.checkBoxCompression.Checked = Settings.Default.Behavior_EnabledCompression;

            // third page:
            this.checkBoxAllowReordering.Checked = Settings.Default.Behavior_AllowColumnReorder;

            this.checkedListBoxColumns.Items.AddRange(
                DataItemBase.GetColumnInfos(DataItemBase.GetPropertyInfos<DataItemBase>()).Select(c => (object)c.Name).ToArray());

            var hidenColumns = new HashSet<string>();
            foreach (var c in Settings.Default.Display_HidenColumns)
            {
                hidenColumns.Add(c);
            }

            for (var i = 0; i < this.checkedListBoxColumns.Items.Count; i++)
                this.checkedListBoxColumns.SetItemChecked(i, !hidenColumns.Contains((string)this.checkedListBoxColumns.Items[i]));

            // hotkeys page
            foreach (var k in HotKeys.Instance)
            {
                this.dataGridViewHotKeys.Rows.Add(k.Value, k.Key);
            }
        }

        private void checkBoxColorLevel_CheckedChanged(object sender, EventArgs e)
        {
            this.propertyGridLevelColor.Enabled = this.checkBoxColorLevel.Checked;
        }

        private void ApplyChanges()
        {
            Settings.Default.Display_ColoredLevel = this.checkBoxColorLevel.Checked;
            Settings.Default.Display_LevelColors = (LevelColors)this.propertyGridLevelColor.SelectedObject;

            Settings.Default.Display_TagColors = (TagColors)this.propertyGridTagColors.SelectedObject;

            Settings.Default.Display_BoldParameter = this.checkBoxBoldParameter.Checked;
            Settings.Default.Display_HighlightFind = this.checkBoxHighlightMatch.Checked;
            Settings.Default.Display_FastRendering = this.checkBoxFastRendering.Checked;

            int indentSize;
            if (int.TryParse(this.textBoxIndentSize.Text, out indentSize))
            {
                Settings.Default.Indent_Size = indentSize;
            }

            // page 2
            Settings.Default.Behavior_AutoLoad = this.checkBoxAutoLoad.Checked;
            Settings.Default.Behavior_AutoScroll = this.checkBoxAutoScroll.Checked;
            Settings.Default.Behavior_DataVirtualization = this.checkBoxDataVirtual.Checked;
            Settings.Default.Behavior_BackgroundInternStrings = this.checkBoxInternStrings.Checked;
            Settings.Default.Behavior_InternIntervalMilliseconds = int.Parse(this.textBoxInternInterval.Text);
            Settings.Default.Behavior_EnabledCompression = this.checkBoxCompression.Checked;

            // page 3
            Settings.Default.Behavior_AllowColumnReorder = this.checkBoxAllowReordering.Checked;

            Settings.Default.Display_HidenColumns.Clear();

            for (var i = 0; i < this.checkedListBoxColumns.Items.Count; i++)
            {
                if (!this.checkedListBoxColumns.GetItemChecked(i))
                {
                    Settings.Default.Display_HidenColumns.Add((string)this.checkedListBoxColumns.Items[i]);
                }
            }

            Settings.Default.Save();
        }

        private void textBoxIndentSize_Validating(object sender, CancelEventArgs e)
        {
            int indentSize;
            e.Cancel = !int.TryParse(this.textBoxIndentSize.Text, out indentSize);
            if (e.Cancel)
            {
                this.errorProvider1.SetError(this.textBoxIndentSize, "The value should be an integer.");
            }
            else
            {
                this.errorProvider1.Clear();
            }
        }

        private void checkBoxInternStrings_CheckedChanged(object sender, EventArgs e)
        {
            this.labelInternInterval.Enabled = this.textBoxInternInterval.Enabled = this.checkBoxInternStrings.Checked;
        }

        private void textBoxInternInterval_Validating(object sender, CancelEventArgs e)
        {
            int v;
            e.Cancel = !(int.TryParse(this.textBoxInternInterval.Text, out v) && v > 0);

            if (e.Cancel)
            {
                this.errorProvider1.SetError(this.textBoxInternInterval, "The value should be an positive integer.");
            }
            else
            {
                this.errorProvider1.Clear();
            }
        }
    }
}
