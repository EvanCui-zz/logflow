namespace SmartViewer
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("<no doc>");
            this.panelTop = new System.Windows.Forms.Panel();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripTextBox1 = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripSplitButton = new System.Windows.Forms.ToolStripSplitButton();
            this.filterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.highlightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panelMain = new System.Windows.Forms.Panel();
            this.panelMiddle = new System.Windows.Forms.Panel();
            this.splitterBottom = new System.Windows.Forms.Splitter();
            this.panelView = new System.Windows.Forms.Panel();
            this.dataGridViewMain = new System.Windows.Forms.DataGridView();
            this.panelDetail = new System.Windows.Forms.Panel();
            this.splitterRight = new System.Windows.Forms.Splitter();
            this.splitterLeft = new System.Windows.Forms.Splitter();
            this.panelRight = new System.Windows.Forms.Panel();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.treeViewDoc = new System.Windows.Forms.TreeView();
            this.contextMenuStripTreeView = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.filterToolStripMenuItemDoc = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripProgressBarStatus = new System.Windows.Forms.ToolStripProgressBar();
            this.progressBarMain = new System.Windows.Forms.ProgressBar();
            this.panelTop.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.menuStripMain.SuspendLayout();
            this.panelMain.SuspendLayout();
            this.panelMiddle.SuspendLayout();
            this.panelView.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewMain)).BeginInit();
            this.panelLeft.SuspendLayout();
            this.contextMenuStripTreeView.SuspendLayout();
            this.statusStripMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelTop
            // 
            this.panelTop.BackColor = System.Drawing.Color.LightSteelBlue;
            this.panelTop.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelTop.Controls.Add(this.toolStripMain);
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 24);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(921, 30);
            this.panelTop.TabIndex = 0;
            // 
            // toolStripMain
            // 
            this.toolStripMain.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripTextBox1,
            this.toolStripSplitButton});
            this.toolStripMain.Location = new System.Drawing.Point(0, 0);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(346, 25);
            this.toolStripMain.TabIndex = 0;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // toolStripTextBox1
            // 
            this.toolStripTextBox1.Name = "toolStripTextBox1";
            this.toolStripTextBox1.Size = new System.Drawing.Size(300, 25);
            // 
            // toolStripSplitButton
            // 
            this.toolStripSplitButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterToolStripMenuItem,
            this.highlightToolStripMenuItem});
            this.toolStripSplitButton.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSplitButton.Image")));
            this.toolStripSplitButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton.Name = "toolStripSplitButton";
            this.toolStripSplitButton.Size = new System.Drawing.Size(32, 22);
            this.toolStripSplitButton.Text = "toolStripSplitButton1";
            this.toolStripSplitButton.ButtonClick += new System.EventHandler(this.toolStripSplitButton_ButtonClick);
            // 
            // filterToolStripMenuItem
            // 
            this.filterToolStripMenuItem.Name = "filterToolStripMenuItem";
            this.filterToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.filterToolStripMenuItem.Text = "Filter";
            this.filterToolStripMenuItem.Click += new System.EventHandler(this.filterToolStripMenuItem_Click);
            // 
            // highlightToolStripMenuItem
            // 
            this.highlightToolStripMenuItem.Name = "highlightToolStripMenuItem";
            this.highlightToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.highlightToolStripMenuItem.Text = "Highlight";
            this.highlightToolStripMenuItem.Click += new System.EventHandler(this.highlightToolStripMenuItem_Click);
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.searchToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(921, 24);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(112, 22);
            this.openToolStripMenuItem.Text = "Open...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // searchToolStripMenuItem
            // 
            this.searchToolStripMenuItem.Name = "searchToolStripMenuItem";
            this.searchToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.searchToolStripMenuItem.Text = "Search";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(42, 20);
            this.toolsToolStripMenuItem.Text = "Tool";
            // 
            // panelMain
            // 
            this.panelMain.Controls.Add(this.panelMiddle);
            this.panelMain.Controls.Add(this.splitterRight);
            this.panelMain.Controls.Add(this.splitterLeft);
            this.panelMain.Controls.Add(this.panelRight);
            this.panelMain.Controls.Add(this.panelLeft);
            this.panelMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMain.Location = new System.Drawing.Point(0, 54);
            this.panelMain.Name = "panelMain";
            this.panelMain.Size = new System.Drawing.Size(921, 414);
            this.panelMain.TabIndex = 7;
            // 
            // panelMiddle
            // 
            this.panelMiddle.Controls.Add(this.splitterBottom);
            this.panelMiddle.Controls.Add(this.panelView);
            this.panelMiddle.Controls.Add(this.panelDetail);
            this.panelMiddle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMiddle.Location = new System.Drawing.Point(163, 0);
            this.panelMiddle.Name = "panelMiddle";
            this.panelMiddle.Size = new System.Drawing.Size(551, 414);
            this.panelMiddle.TabIndex = 4;
            // 
            // splitterBottom
            // 
            this.splitterBottom.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.splitterBottom.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.splitterBottom.Location = new System.Drawing.Point(0, 315);
            this.splitterBottom.Name = "splitterBottom";
            this.splitterBottom.Size = new System.Drawing.Size(551, 7);
            this.splitterBottom.TabIndex = 2;
            this.splitterBottom.TabStop = false;
            // 
            // panelView
            // 
            this.panelView.BackColor = System.Drawing.Color.Thistle;
            this.panelView.Controls.Add(this.progressBarMain);
            this.panelView.Controls.Add(this.dataGridViewMain);
            this.panelView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelView.Location = new System.Drawing.Point(0, 0);
            this.panelView.Name = "panelView";
            this.panelView.Size = new System.Drawing.Size(551, 322);
            this.panelView.TabIndex = 1;
            // 
            // dataGridViewMain
            // 
            this.dataGridViewMain.AllowUserToAddRows = false;
            this.dataGridViewMain.AllowUserToDeleteRows = false;
            this.dataGridViewMain.AllowUserToOrderColumns = true;
            this.dataGridViewMain.AllowUserToResizeRows = false;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.Color.Blue;
            this.dataGridViewMain.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.dataGridViewMain.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.DisplayedCells;
            this.dataGridViewMain.BackgroundColor = System.Drawing.Color.White;
            this.dataGridViewMain.CausesValidation = false;
            this.dataGridViewMain.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.SingleHorizontal;
            this.dataGridViewMain.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewMain.Location = new System.Drawing.Point(0, 0);
            this.dataGridViewMain.Name = "dataGridViewMain";
            this.dataGridViewMain.ReadOnly = true;
            this.dataGridViewMain.RowHeadersVisible = false;
            this.dataGridViewMain.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dataGridViewMain.Size = new System.Drawing.Size(551, 322);
            this.dataGridViewMain.TabIndex = 0;
            this.dataGridViewMain.VirtualMode = true;
            this.dataGridViewMain.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridViewMain_CellContentClick);
            this.dataGridViewMain.CellPainting += new System.Windows.Forms.DataGridViewCellPaintingEventHandler(this.dataGridViewMain_CellPainting);
            this.dataGridViewMain.CellValueNeeded += new System.Windows.Forms.DataGridViewCellValueEventHandler(this.dataGridViewMain_CellValueNeeded);
            this.dataGridViewMain.RowPrePaint += new System.Windows.Forms.DataGridViewRowPrePaintEventHandler(this.dataGridViewMain_RowPrePaint);
            // 
            // panelDetail
            // 
            this.panelDetail.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panelDetail.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelDetail.Location = new System.Drawing.Point(0, 322);
            this.panelDetail.Name = "panelDetail";
            this.panelDetail.Size = new System.Drawing.Size(551, 92);
            this.panelDetail.TabIndex = 0;
            // 
            // splitterRight
            // 
            this.splitterRight.BackColor = System.Drawing.Color.LightSteelBlue;
            this.splitterRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.splitterRight.Location = new System.Drawing.Point(714, 0);
            this.splitterRight.Name = "splitterRight";
            this.splitterRight.Size = new System.Drawing.Size(7, 414);
            this.splitterRight.TabIndex = 3;
            this.splitterRight.TabStop = false;
            // 
            // splitterLeft
            // 
            this.splitterLeft.BackColor = System.Drawing.Color.LightSteelBlue;
            this.splitterLeft.Location = new System.Drawing.Point(156, 0);
            this.splitterLeft.Name = "splitterLeft";
            this.splitterLeft.Size = new System.Drawing.Size(7, 414);
            this.splitterLeft.TabIndex = 2;
            this.splitterLeft.TabStop = false;
            // 
            // panelRight
            // 
            this.panelRight.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.panelRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.panelRight.Location = new System.Drawing.Point(721, 0);
            this.panelRight.Name = "panelRight";
            this.panelRight.Size = new System.Drawing.Size(200, 414);
            this.panelRight.TabIndex = 1;
            // 
            // panelLeft
            // 
            this.panelLeft.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.panelLeft.Controls.Add(this.treeViewDoc);
            this.panelLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLeft.Location = new System.Drawing.Point(0, 0);
            this.panelLeft.Name = "panelLeft";
            this.panelLeft.Size = new System.Drawing.Size(156, 414);
            this.panelLeft.TabIndex = 0;
            // 
            // treeViewDoc
            // 
            this.treeViewDoc.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.treeViewDoc.ContextMenuStrip = this.contextMenuStripTreeView;
            this.treeViewDoc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewDoc.FullRowSelect = true;
            this.treeViewDoc.HideSelection = false;
            this.treeViewDoc.Location = new System.Drawing.Point(0, 0);
            this.treeViewDoc.Name = "treeViewDoc";
            treeNode1.Name = "Node0";
            treeNode1.Text = "<no doc>";
            this.treeViewDoc.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode1});
            this.treeViewDoc.ShowLines = false;
            this.treeViewDoc.Size = new System.Drawing.Size(156, 414);
            this.treeViewDoc.TabIndex = 0;
            this.treeViewDoc.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewDoc_AfterSelect);
            // 
            // contextMenuStripTreeView
            // 
            this.contextMenuStripTreeView.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterToolStripMenuItemDoc});
            this.contextMenuStripTreeView.Name = "contextMenuStripTreeView";
            this.contextMenuStripTreeView.Size = new System.Drawing.Size(110, 26);
            // 
            // filterToolStripMenuItemDoc
            // 
            this.filterToolStripMenuItemDoc.Name = "filterToolStripMenuItemDoc";
            this.filterToolStripMenuItemDoc.Size = new System.Drawing.Size(109, 22);
            this.filterToolStripMenuItemDoc.Text = "Filter...";
            this.filterToolStripMenuItemDoc.Click += new System.EventHandler(this.filterToolStripMenuItemDoc_Click);
            // 
            // statusStripMain
            // 
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel,
            this.toolStripProgressBarStatus});
            this.statusStripMain.Location = new System.Drawing.Point(0, 446);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(921, 22);
            this.statusStripMain.TabIndex = 8;
            this.statusStripMain.Text = "statusStrip1";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusLabel.Text = "Ready";
            // 
            // toolStripProgressBarStatus
            // 
            this.toolStripProgressBarStatus.Name = "toolStripProgressBarStatus";
            this.toolStripProgressBarStatus.Size = new System.Drawing.Size(100, 16);
            // 
            // progressBarMain
            // 
            this.progressBarMain.Dock = System.Windows.Forms.DockStyle.Top;
            this.progressBarMain.Location = new System.Drawing.Point(0, 0);
            this.progressBarMain.Name = "progressBarMain";
            this.progressBarMain.Size = new System.Drawing.Size(551, 5);
            this.progressBarMain.TabIndex = 1;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(921, 468);
            this.Controls.Add(this.statusStripMain);
            this.Controls.Add(this.panelMain);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.menuStripMain);
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "MainForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.panelMain.ResumeLayout(false);
            this.panelMiddle.ResumeLayout(false);
            this.panelView.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewMain)).EndInit();
            this.panelLeft.ResumeLayout(false);
            this.contextMenuStripTreeView.ResumeLayout(false);
            this.statusStripMain.ResumeLayout(false);
            this.statusStripMain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panelTop;
        private System.Windows.Forms.MenuStrip menuStripMain;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem searchToolStripMenuItem;
        private System.Windows.Forms.Panel panelMain;
        private System.Windows.Forms.Panel panelMiddle;
        private System.Windows.Forms.Panel panelView;
        private System.Windows.Forms.Panel panelDetail;
        private System.Windows.Forms.Splitter splitterRight;
        private System.Windows.Forms.Splitter splitterLeft;
        private System.Windows.Forms.Panel panelRight;
        private System.Windows.Forms.Panel panelLeft;
        private System.Windows.Forms.Splitter splitterBottom;
        private System.Windows.Forms.DataGridView dataGridViewMain;
        private System.Windows.Forms.TreeView treeViewDoc;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripTreeView;
        private System.Windows.Forms.ToolStripMenuItem filterToolStripMenuItemDoc;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBox1;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButton;
        private System.Windows.Forms.ToolStripMenuItem filterToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem highlightToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBarStatus;
        private System.Windows.Forms.ProgressBar progressBarMain;
    }
}

