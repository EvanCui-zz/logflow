namespace LogFlow.Viewer
{
    partial class AllSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageDisplay = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxIndentSize = new System.Windows.Forms.TextBox();
            this.checkBoxFastRendering = new System.Windows.Forms.CheckBox();
            this.checkBoxHighlightMatch = new System.Windows.Forms.CheckBox();
            this.checkBoxBoldParameter = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.propertyGridTagColors = new System.Windows.Forms.PropertyGrid();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.propertyGridLevelColor = new System.Windows.Forms.PropertyGrid();
            this.checkBoxColorLevel = new System.Windows.Forms.CheckBox();
            this.tabPageBehavior = new System.Windows.Forms.TabPage();
            this.checkBoxDataVirtual = new System.Windows.Forms.CheckBox();
            this.checkBoxAutoScroll = new System.Windows.Forms.CheckBox();
            this.checkBoxAutoLoad = new System.Windows.Forms.CheckBox();
            this.tabPageColumns = new System.Windows.Forms.TabPage();
            this.checkBoxAllowReordering = new System.Windows.Forms.CheckBox();
            this.checkedListBoxColumns = new System.Windows.Forms.CheckedListBox();
            this.tabPageHotKey = new System.Windows.Forms.TabPage();
            this.dataGridViewHotKeys = new System.Windows.Forms.DataGridView();
            this.Functionality = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.HotKey = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.buttonApply = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.tabControl1.SuspendLayout();
            this.tabPageDisplay.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPageBehavior.SuspendLayout();
            this.tabPageColumns.SuspendLayout();
            this.tabPageHotKey.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewHotKeys)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPageDisplay);
            this.tabControl1.Controls.Add(this.tabPageBehavior);
            this.tabControl1.Controls.Add(this.tabPageColumns);
            this.tabControl1.Controls.Add(this.tabPageHotKey);
            this.tabControl1.ItemSize = new System.Drawing.Size(58, 19);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(470, 436);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageDisplay
            // 
            this.tabPageDisplay.Controls.Add(this.label1);
            this.tabPageDisplay.Controls.Add(this.textBoxIndentSize);
            this.tabPageDisplay.Controls.Add(this.checkBoxFastRendering);
            this.tabPageDisplay.Controls.Add(this.checkBoxHighlightMatch);
            this.tabPageDisplay.Controls.Add(this.checkBoxBoldParameter);
            this.tabPageDisplay.Controls.Add(this.groupBox2);
            this.tabPageDisplay.Controls.Add(this.groupBox1);
            this.tabPageDisplay.Location = new System.Drawing.Point(4, 23);
            this.tabPageDisplay.Name = "tabPageDisplay";
            this.tabPageDisplay.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageDisplay.Size = new System.Drawing.Size(462, 409);
            this.tabPageDisplay.TabIndex = 0;
            this.tabPageDisplay.Text = "Display";
            this.tabPageDisplay.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 375);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Indent size";
            // 
            // textBoxIndentSize
            // 
            this.textBoxIndentSize.Location = new System.Drawing.Point(85, 372);
            this.textBoxIndentSize.Name = "textBoxIndentSize";
            this.textBoxIndentSize.Size = new System.Drawing.Size(100, 20);
            this.textBoxIndentSize.TabIndex = 6;
            this.textBoxIndentSize.Validating += new System.ComponentModel.CancelEventHandler(this.textBoxIndentSize_Validating);
            // 
            // checkBoxFastRendering
            // 
            this.checkBoxFastRendering.AutoSize = true;
            this.checkBoxFastRendering.Location = new System.Drawing.Point(18, 341);
            this.checkBoxFastRendering.Name = "checkBoxFastRendering";
            this.checkBoxFastRendering.Size = new System.Drawing.Size(422, 17);
            this.checkBoxFastRendering.TabIndex = 5;
            this.checkBoxFastRendering.Text = "Enable fast rendering. (Check only when the list view flicks too much when scroll" +
    "ing)";
            this.checkBoxFastRendering.UseVisualStyleBackColor = true;
            // 
            // checkBoxHighlightMatch
            // 
            this.checkBoxHighlightMatch.AutoSize = true;
            this.checkBoxHighlightMatch.Location = new System.Drawing.Point(18, 318);
            this.checkBoxHighlightMatch.Name = "checkBoxHighlightMatch";
            this.checkBoxHighlightMatch.Size = new System.Drawing.Size(230, 17);
            this.checkBoxHighlightMatch.TabIndex = 4;
            this.checkBoxHighlightMatch.Text = "Highlight the matched text when searching.";
            this.checkBoxHighlightMatch.UseVisualStyleBackColor = true;
            // 
            // checkBoxBoldParameter
            // 
            this.checkBoxBoldParameter.AutoSize = true;
            this.checkBoxBoldParameter.Location = new System.Drawing.Point(18, 295);
            this.checkBoxBoldParameter.Name = "checkBoxBoldParameter";
            this.checkBoxBoldParameter.Size = new System.Drawing.Size(173, 17);
            this.checkBoxBoldParameter.TabIndex = 3;
            this.checkBoxBoldParameter.Text = "Display parameters in bold font.";
            this.checkBoxBoldParameter.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.propertyGridTagColors);
            this.groupBox2.Location = new System.Drawing.Point(12, 187);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(436, 92);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tag Colors";
            // 
            // propertyGridTagColors
            // 
            this.propertyGridTagColors.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGridTagColors.HelpVisible = false;
            this.propertyGridTagColors.Location = new System.Drawing.Point(6, 19);
            this.propertyGridTagColors.Name = "propertyGridTagColors";
            this.propertyGridTagColors.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.propertyGridTagColors.Size = new System.Drawing.Size(424, 67);
            this.propertyGridTagColors.TabIndex = 0;
            this.propertyGridTagColors.ToolbarVisible = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.propertyGridLevelColor);
            this.groupBox1.Controls.Add(this.checkBoxColorLevel);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(436, 161);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Level Colors";
            // 
            // propertyGridLevelColor
            // 
            this.propertyGridLevelColor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGridLevelColor.HelpVisible = false;
            this.propertyGridLevelColor.Location = new System.Drawing.Point(6, 42);
            this.propertyGridLevelColor.Name = "propertyGridLevelColor";
            this.propertyGridLevelColor.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.propertyGridLevelColor.Size = new System.Drawing.Size(424, 113);
            this.propertyGridLevelColor.TabIndex = 1;
            this.propertyGridLevelColor.ToolbarVisible = false;
            // 
            // checkBoxColorLevel
            // 
            this.checkBoxColorLevel.AutoSize = true;
            this.checkBoxColorLevel.Location = new System.Drawing.Point(6, 19);
            this.checkBoxColorLevel.Name = "checkBoxColorLevel";
            this.checkBoxColorLevel.Size = new System.Drawing.Size(362, 17);
            this.checkBoxColorLevel.TabIndex = 0;
            this.checkBoxColorLevel.Text = "Display log lines in customized background colors according to its level.";
            this.checkBoxColorLevel.UseVisualStyleBackColor = true;
            this.checkBoxColorLevel.CheckedChanged += new System.EventHandler(this.checkBoxColorLevel_CheckedChanged);
            // 
            // tabPageBehavior
            // 
            this.tabPageBehavior.Controls.Add(this.checkBoxDataVirtual);
            this.tabPageBehavior.Controls.Add(this.checkBoxAutoScroll);
            this.tabPageBehavior.Controls.Add(this.checkBoxAutoLoad);
            this.tabPageBehavior.Location = new System.Drawing.Point(4, 23);
            this.tabPageBehavior.Name = "tabPageBehavior";
            this.tabPageBehavior.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBehavior.Size = new System.Drawing.Size(462, 409);
            this.tabPageBehavior.TabIndex = 1;
            this.tabPageBehavior.Text = "Behavior";
            this.tabPageBehavior.UseVisualStyleBackColor = true;
            // 
            // checkBoxDataVirtual
            // 
            this.checkBoxDataVirtual.AutoSize = true;
            this.checkBoxDataVirtual.Location = new System.Drawing.Point(18, 64);
            this.checkBoxDataVirtual.Name = "checkBoxDataVirtual";
            this.checkBoxDataVirtual.Size = new System.Drawing.Size(439, 17);
            this.checkBoxDataVirtual.TabIndex = 2;
            this.checkBoxDataVirtual.Text = "Enable data virtualization. (Only enable when a large set of log entries are to b" +
    "e loaded.)";
            this.checkBoxDataVirtual.UseVisualStyleBackColor = true;
            // 
            // checkBoxAutoScroll
            // 
            this.checkBoxAutoScroll.AutoSize = true;
            this.checkBoxAutoScroll.Location = new System.Drawing.Point(18, 41);
            this.checkBoxAutoScroll.Name = "checkBoxAutoScroll";
            this.checkBoxAutoScroll.Size = new System.Drawing.Size(288, 17);
            this.checkBoxAutoScroll.TabIndex = 1;
            this.checkBoxAutoScroll.Text = "Automatically scroll to the end when new entries added.";
            this.checkBoxAutoScroll.UseVisualStyleBackColor = true;
            // 
            // checkBoxAutoLoad
            // 
            this.checkBoxAutoLoad.AutoSize = true;
            this.checkBoxAutoLoad.Location = new System.Drawing.Point(18, 18);
            this.checkBoxAutoLoad.Name = "checkBoxAutoLoad";
            this.checkBoxAutoLoad.Size = new System.Drawing.Size(303, 17);
            this.checkBoxAutoLoad.TabIndex = 0;
            this.checkBoxAutoLoad.Text = "Automatically load new entries from the opened log source.";
            this.checkBoxAutoLoad.UseVisualStyleBackColor = true;
            // 
            // tabPageColumns
            // 
            this.tabPageColumns.Controls.Add(this.checkBoxAllowReordering);
            this.tabPageColumns.Controls.Add(this.checkedListBoxColumns);
            this.tabPageColumns.Location = new System.Drawing.Point(4, 23);
            this.tabPageColumns.Name = "tabPageColumns";
            this.tabPageColumns.Size = new System.Drawing.Size(462, 409);
            this.tabPageColumns.TabIndex = 2;
            this.tabPageColumns.Text = "Column Selection";
            this.tabPageColumns.UseVisualStyleBackColor = true;
            // 
            // checkBoxAllowReordering
            // 
            this.checkBoxAllowReordering.AutoSize = true;
            this.checkBoxAllowReordering.Location = new System.Drawing.Point(18, 188);
            this.checkBoxAllowReordering.Name = "checkBoxAllowReordering";
            this.checkBoxAllowReordering.Size = new System.Drawing.Size(141, 17);
            this.checkBoxAllowReordering.TabIndex = 5;
            this.checkBoxAllowReordering.Text = "Allow column reordering.";
            this.checkBoxAllowReordering.UseVisualStyleBackColor = true;
            // 
            // checkedListBoxColumns
            // 
            this.checkedListBoxColumns.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.checkedListBoxColumns.FormattingEnabled = true;
            this.checkedListBoxColumns.Location = new System.Drawing.Point(18, 18);
            this.checkedListBoxColumns.Name = "checkedListBoxColumns";
            this.checkedListBoxColumns.Size = new System.Drawing.Size(428, 154);
            this.checkedListBoxColumns.TabIndex = 4;
            // 
            // tabPageHotKey
            // 
            this.tabPageHotKey.Controls.Add(this.dataGridViewHotKeys);
            this.tabPageHotKey.Location = new System.Drawing.Point(4, 23);
            this.tabPageHotKey.Name = "tabPageHotKey";
            this.tabPageHotKey.Size = new System.Drawing.Size(462, 409);
            this.tabPageHotKey.TabIndex = 3;
            this.tabPageHotKey.Text = "Hot Keys";
            this.tabPageHotKey.UseVisualStyleBackColor = true;
            // 
            // dataGridViewHotKeys
            // 
            this.dataGridViewHotKeys.AllowUserToAddRows = false;
            this.dataGridViewHotKeys.AllowUserToDeleteRows = false;
            this.dataGridViewHotKeys.AllowUserToResizeRows = false;
            this.dataGridViewHotKeys.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.dataGridViewHotKeys.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewHotKeys.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Functionality,
            this.HotKey});
            this.dataGridViewHotKeys.Location = new System.Drawing.Point(18, 18);
            this.dataGridViewHotKeys.Name = "dataGridViewHotKeys";
            this.dataGridViewHotKeys.ReadOnly = true;
            this.dataGridViewHotKeys.RowHeadersVisible = false;
            this.dataGridViewHotKeys.Size = new System.Drawing.Size(425, 373);
            this.dataGridViewHotKeys.TabIndex = 0;
            // 
            // Functionality
            // 
            this.Functionality.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Functionality.HeaderText = "Functionality";
            this.Functionality.Name = "Functionality";
            this.Functionality.ReadOnly = true;
            // 
            // HotKey
            // 
            this.HotKey.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.HotKey.HeaderText = "Hot Keys";
            this.HotKey.Name = "HotKey";
            this.HotKey.ReadOnly = true;
            // 
            // buttonApply
            // 
            this.buttonApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonApply.Location = new System.Drawing.Point(403, 454);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(75, 23);
            this.buttonApply.TabIndex = 1;
            this.buttonApply.Text = "Apply";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(322, 454);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOK.Location = new System.Drawing.Point(241, 454);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 3;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // AllSettings
            // 
            this.AcceptButton = this.buttonOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(494, 489);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonApply);
            this.Controls.Add(this.tabControl1);
            this.DoubleBuffered = true;
            this.Name = "AllSettings";
            this.Text = "Settings";
            this.Load += new System.EventHandler(this.AllSettings_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPageDisplay.ResumeLayout(false);
            this.tabPageDisplay.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPageBehavior.ResumeLayout(false);
            this.tabPageBehavior.PerformLayout();
            this.tabPageColumns.ResumeLayout(false);
            this.tabPageColumns.PerformLayout();
            this.tabPageHotKey.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewHotKeys)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageDisplay;
        private System.Windows.Forms.TabPage tabPageBehavior;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.PropertyGrid propertyGridLevelColor;
        private System.Windows.Forms.CheckBox checkBoxColorLevel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxIndentSize;
        private System.Windows.Forms.CheckBox checkBoxFastRendering;
        private System.Windows.Forms.CheckBox checkBoxHighlightMatch;
        private System.Windows.Forms.CheckBox checkBoxBoldParameter;
        private System.Windows.Forms.PropertyGrid propertyGridTagColors;
        private System.Windows.Forms.CheckBox checkBoxDataVirtual;
        private System.Windows.Forms.CheckBox checkBoxAutoScroll;
        private System.Windows.Forms.CheckBox checkBoxAutoLoad;
        private System.Windows.Forms.TabPage tabPageColumns;
        private System.Windows.Forms.CheckBox checkBoxAllowReordering;
        private System.Windows.Forms.CheckedListBox checkedListBoxColumns;
        private System.Windows.Forms.TabPage tabPageHotKey;
        private System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.DataGridView dataGridViewHotKeys;
        private System.Windows.Forms.DataGridViewTextBoxColumn Functionality;
        private System.Windows.Forms.DataGridViewTextBoxColumn HotKey;
        private System.Windows.Forms.ErrorProvider errorProvider1;
    }
}