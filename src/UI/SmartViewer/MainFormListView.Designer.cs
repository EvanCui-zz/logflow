namespace SmartViewer
{
    partial class MainFormListView
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
                this.backgroundWorker1.Dispose();
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainFormListView));
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("<no doc>");
            this.panelTop = new System.Windows.Forms.Panel();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripTextBoxPattern = new System.Windows.Forms.ToolStripTextBox();
            this.toolStripButtonFilter = new System.Windows.Forms.ToolStripButton();
            this.toolStripSplitButtonTag = new System.Windows.Forms.ToolStripSplitButton();
            this.toolStripSplitButtonFind = new System.Windows.Forms.ToolStripSplitButton();
            this.findPreviousToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findNextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripButtonCount = new System.Windows.Forms.ToolStripButton();
            this.toolStripLabelCount = new System.Windows.Forms.ToolStripLabel();
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterF1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tagF2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchF3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.countF4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panelMain = new System.Windows.Forms.Panel();
            this.panelMiddle = new System.Windows.Forms.Panel();
            this.splitContainerDetails = new System.Windows.Forms.SplitContainer();
            this.progressBarMain = new System.Windows.Forms.ProgressBar();
            this.tabControlDetail = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.splitContainerDetail = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.labelProcessId = new System.Windows.Forms.Label();
            this.labelThreadId = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelTime = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.labelLevel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.labelId = new System.Windows.Forms.Label();
            this.textBoxText = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.label14 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.splitterRight = new System.Windows.Forms.Splitter();
            this.splitterLeft = new System.Windows.Forms.Splitter();
            this.panelRight = new System.Windows.Forms.Panel();
            this.splitContainerStatistics = new System.Windows.Forms.SplitContainer();
            this.chartTimeLine = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.propertyGridStatistics = new System.Windows.Forms.PropertyGrid();
            this.splitterPlaceHolder = new System.Windows.Forms.Splitter();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.treeViewDoc = new System.Windows.Forms.TreeView();
            this.contextMenuStripTreeView = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.filterToolStripMenuItemDoc = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelFill = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelSelected = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.fastListViewMain = new SmartViewer.FastListView();
            this.timeline1 = new SmartViewer.Timeline();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.panelTop.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.menuStripMain.SuspendLayout();
            this.panelMain.SuspendLayout();
            this.panelMiddle.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerDetails)).BeginInit();
            this.splitContainerDetails.Panel1.SuspendLayout();
            this.splitContainerDetails.Panel2.SuspendLayout();
            this.splitContainerDetails.SuspendLayout();
            this.tabControlDetail.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerDetail)).BeginInit();
            this.splitContainerDetail.Panel1.SuspendLayout();
            this.splitContainerDetail.Panel2.SuspendLayout();
            this.splitContainerDetail.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.panelRight.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerStatistics)).BeginInit();
            this.splitContainerStatistics.Panel1.SuspendLayout();
            this.splitContainerStatistics.Panel2.SuspendLayout();
            this.splitContainerStatistics.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chartTimeLine)).BeginInit();
            this.panelLeft.SuspendLayout();
            this.contextMenuStripTreeView.SuspendLayout();
            this.statusStripMain.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelTop
            // 
            this.panelTop.BackColor = System.Drawing.Color.WhiteSmoke;
            this.panelTop.Controls.Add(this.toolStripMain);
            this.panelTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelTop.Location = new System.Drawing.Point(0, 24);
            this.panelTop.Name = "panelTop";
            this.panelTop.Size = new System.Drawing.Size(1022, 30);
            this.panelTop.TabIndex = 0;
            // 
            // toolStripMain
            // 
            this.toolStripMain.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripTextBoxPattern,
            this.toolStripButtonFilter,
            this.toolStripSplitButtonTag,
            this.toolStripSplitButtonFind,
            this.toolStripButtonCount,
            this.toolStripLabelCount});
            this.toolStripMain.Location = new System.Drawing.Point(0, 0);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(657, 25);
            this.toolStripMain.TabIndex = 0;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // toolStripTextBoxPattern
            // 
            this.toolStripTextBoxPattern.Name = "toolStripTextBoxPattern";
            this.toolStripTextBoxPattern.Size = new System.Drawing.Size(300, 25);
            this.toolStripTextBoxPattern.TextChanged += new System.EventHandler(this.toolStripTextBoxPattern_TextChanged);
            // 
            // toolStripButtonFilter
            // 
            this.toolStripButtonFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonFilter.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonFilter.Image")));
            this.toolStripButtonFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonFilter.Name = "toolStripButtonFilter";
            this.toolStripButtonFilter.Size = new System.Drawing.Size(60, 22);
            this.toolStripButtonFilter.Text = "Filter (F1)";
            this.toolStripButtonFilter.Click += new System.EventHandler(this.toolStripButtonFilter_Click);
            // 
            // toolStripSplitButtonTag
            // 
            this.toolStripSplitButtonTag.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripSplitButtonTag.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButtonTag.Name = "toolStripSplitButtonTag";
            this.toolStripSplitButtonTag.Size = new System.Drawing.Size(65, 22);
            this.toolStripSplitButtonTag.Text = "Tag (F2)";
            // 
            // toolStripSplitButtonFind
            // 
            this.toolStripSplitButtonFind.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripSplitButtonFind.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.findPreviousToolStripMenuItem,
            this.findNextToolStripMenuItem});
            this.toolStripSplitButtonFind.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSplitButtonFind.Image")));
            this.toolStripSplitButtonFind.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButtonFind.Name = "toolStripSplitButtonFind";
            this.toolStripSplitButtonFind.Size = new System.Drawing.Size(81, 22);
            this.toolStripSplitButtonFind.Text = "Search (F3)";
            // 
            // findPreviousToolStripMenuItem
            // 
            this.findPreviousToolStripMenuItem.Name = "findPreviousToolStripMenuItem";
            this.findPreviousToolStripMenuItem.Size = new System.Drawing.Size(218, 22);
            this.findPreviousToolStripMenuItem.Text = "Search Previous (Shift + F3)";
            this.findPreviousToolStripMenuItem.Click += new System.EventHandler(this.findPreviousToolStripMenuItem_Click);
            // 
            // findNextToolStripMenuItem
            // 
            this.findNextToolStripMenuItem.Name = "findNextToolStripMenuItem";
            this.findNextToolStripMenuItem.Size = new System.Drawing.Size(218, 22);
            this.findNextToolStripMenuItem.Text = "Search Next (F3)";
            this.findNextToolStripMenuItem.Click += new System.EventHandler(this.findNextToolStripMenuItem_Click);
            // 
            // toolStripButtonCount
            // 
            this.toolStripButtonCount.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonCount.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonCount.Image")));
            this.toolStripButtonCount.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonCount.Name = "toolStripButtonCount";
            this.toolStripButtonCount.Size = new System.Drawing.Size(67, 22);
            this.toolStripButtonCount.Text = "Count (F4)";
            this.toolStripButtonCount.Click += new System.EventHandler(this.toolStripButtonCount_Click);
            // 
            // toolStripLabelCount
            // 
            this.toolStripLabelCount.AutoSize = false;
            this.toolStripLabelCount.Name = "toolStripLabelCount";
            this.toolStripLabelCount.Size = new System.Drawing.Size(70, 22);
            this.toolStripLabelCount.Text = "0";
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.searchToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(1022, 24);
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
            this.searchToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterF1ToolStripMenuItem,
            this.tagF2ToolStripMenuItem,
            this.searchF3ToolStripMenuItem,
            this.countF4ToolStripMenuItem});
            this.searchToolStripMenuItem.Name = "searchToolStripMenuItem";
            this.searchToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.searchToolStripMenuItem.Text = "Analyze";
            // 
            // filterF1ToolStripMenuItem
            // 
            this.filterF1ToolStripMenuItem.Name = "filterF1ToolStripMenuItem";
            this.filterF1ToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.filterF1ToolStripMenuItem.Text = "Filter (F1)";
            // 
            // tagF2ToolStripMenuItem
            // 
            this.tagF2ToolStripMenuItem.Name = "tagF2ToolStripMenuItem";
            this.tagF2ToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.tagF2ToolStripMenuItem.Text = "Tag (F2)";
            // 
            // searchF3ToolStripMenuItem
            // 
            this.searchF3ToolStripMenuItem.Name = "searchF3ToolStripMenuItem";
            this.searchF3ToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.searchF3ToolStripMenuItem.Text = "Search (F3)";
            // 
            // countF4ToolStripMenuItem
            // 
            this.countF4ToolStripMenuItem.Name = "countF4ToolStripMenuItem";
            this.countF4ToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
            this.countF4ToolStripMenuItem.Text = "Count (F4)";
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
            this.panelMain.Size = new System.Drawing.Size(1022, 485);
            this.panelMain.TabIndex = 7;
            // 
            // panelMiddle
            // 
            this.panelMiddle.Controls.Add(this.splitContainerDetails);
            this.panelMiddle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelMiddle.Location = new System.Drawing.Point(163, 0);
            this.panelMiddle.Name = "panelMiddle";
            this.panelMiddle.Size = new System.Drawing.Size(691, 485);
            this.panelMiddle.TabIndex = 4;
            // 
            // splitContainerDetails
            // 
            this.splitContainerDetails.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitContainerDetails.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerDetails.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainerDetails.Location = new System.Drawing.Point(0, 0);
            this.splitContainerDetails.Name = "splitContainerDetails";
            this.splitContainerDetails.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerDetails.Panel1
            // 
            this.splitContainerDetails.Panel1.Controls.Add(this.fastListViewMain);
            this.splitContainerDetails.Panel1.Controls.Add(this.progressBarMain);
            // 
            // splitContainerDetails.Panel2
            // 
            this.splitContainerDetails.Panel2.Controls.Add(this.tabControlDetail);
            this.splitContainerDetails.Size = new System.Drawing.Size(691, 485);
            this.splitContainerDetails.SplitterDistance = 284;
            this.splitContainerDetails.TabIndex = 2;
            // 
            // progressBarMain
            // 
            this.progressBarMain.Dock = System.Windows.Forms.DockStyle.Top;
            this.progressBarMain.Location = new System.Drawing.Point(0, 0);
            this.progressBarMain.Name = "progressBarMain";
            this.progressBarMain.Size = new System.Drawing.Size(689, 5);
            this.progressBarMain.TabIndex = 1;
            // 
            // tabControlDetail
            // 
            this.tabControlDetail.Appearance = System.Windows.Forms.TabAppearance.FlatButtons;
            this.tabControlDetail.Controls.Add(this.tabPage1);
            this.tabControlDetail.Controls.Add(this.tabPage2);
            this.tabControlDetail.Controls.Add(this.tabPage3);
            this.tabControlDetail.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlDetail.Location = new System.Drawing.Point(0, 0);
            this.tabControlDetail.Name = "tabControlDetail";
            this.tabControlDetail.SelectedIndex = 0;
            this.tabControlDetail.Size = new System.Drawing.Size(689, 195);
            this.tabControlDetail.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.White;
            this.tabPage1.Controls.Add(this.splitContainerDetail);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(681, 166);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Details";
            // 
            // splitContainerDetail
            // 
            this.splitContainerDetail.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerDetail.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainerDetail.Location = new System.Drawing.Point(3, 3);
            this.splitContainerDetail.Name = "splitContainerDetail";
            // 
            // splitContainerDetail.Panel1
            // 
            this.splitContainerDetail.Panel1.AutoScroll = true;
            this.splitContainerDetail.Panel1.Controls.Add(this.groupBox1);
            this.splitContainerDetail.Panel1.Controls.Add(this.labelProcessId);
            this.splitContainerDetail.Panel1.Controls.Add(this.labelThreadId);
            this.splitContainerDetail.Panel1.Controls.Add(this.label4);
            this.splitContainerDetail.Panel1.Controls.Add(this.label3);
            this.splitContainerDetail.Panel1.Controls.Add(this.label2);
            this.splitContainerDetail.Panel1.Controls.Add(this.labelTime);
            this.splitContainerDetail.Panel1.Controls.Add(this.label5);
            this.splitContainerDetail.Panel1.Controls.Add(this.labelLevel);
            this.splitContainerDetail.Panel1.Controls.Add(this.label1);
            this.splitContainerDetail.Panel1.Controls.Add(this.labelId);
            // 
            // splitContainerDetail.Panel2
            // 
            this.splitContainerDetail.Panel2.AutoScroll = true;
            this.splitContainerDetail.Panel2.Controls.Add(this.textBoxText);
            this.splitContainerDetail.Size = new System.Drawing.Size(675, 160);
            this.splitContainerDetail.SplitterDistance = 226;
            this.splitContainerDetail.TabIndex = 4;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Location = new System.Drawing.Point(216, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(2, 141);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // labelProcessId
            // 
            this.labelProcessId.Location = new System.Drawing.Point(88, 112);
            this.labelProcessId.Name = "labelProcessId";
            this.labelProcessId.Size = new System.Drawing.Size(128, 16);
            this.labelProcessId.TabIndex = 14;
            // 
            // labelThreadId
            // 
            this.labelThreadId.Location = new System.Drawing.Point(88, 88);
            this.labelThreadId.Name = "labelThreadId";
            this.labelThreadId.Size = new System.Drawing.Size(128, 16);
            this.labelThreadId.TabIndex = 15;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(16, 112);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 16);
            this.label4.TabIndex = 12;
            this.label4.Text = "Process Id:";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(16, 88);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(72, 16);
            this.label3.TabIndex = 13;
            this.label3.Text = "Thread Id:";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(16, 64);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 16);
            this.label2.TabIndex = 8;
            this.label2.Text = "Time:";
            // 
            // labelTime
            // 
            this.labelTime.Location = new System.Drawing.Point(88, 64);
            this.labelTime.Name = "labelTime";
            this.labelTime.Size = new System.Drawing.Size(128, 16);
            this.labelTime.TabIndex = 10;
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(16, 40);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(72, 16);
            this.label5.TabIndex = 11;
            this.label5.Text = "Level:";
            // 
            // labelLevel
            // 
            this.labelLevel.Location = new System.Drawing.Point(88, 40);
            this.labelLevel.Name = "labelLevel";
            this.labelLevel.Size = new System.Drawing.Size(128, 16);
            this.labelLevel.TabIndex = 9;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(16, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 16);
            this.label1.TabIndex = 6;
            this.label1.Text = "Id:";
            // 
            // labelId
            // 
            this.labelId.Location = new System.Drawing.Point(88, 16);
            this.labelId.Name = "labelId";
            this.labelId.Size = new System.Drawing.Size(128, 16);
            this.labelId.TabIndex = 7;
            // 
            // textBoxText
            // 
            this.textBoxText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxText.BackColor = System.Drawing.Color.White;
            this.textBoxText.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBoxText.Location = new System.Drawing.Point(8, 16);
            this.textBoxText.Multiline = true;
            this.textBoxText.Name = "textBoxText";
            this.textBoxText.ReadOnly = true;
            this.textBoxText.Size = new System.Drawing.Size(429, 132);
            this.textBoxText.TabIndex = 1;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label11);
            this.tabPage2.Controls.Add(this.label10);
            this.tabPage2.Controls.Add(this.label9);
            this.tabPage2.Controls.Add(this.label8);
            this.tabPage2.Controls.Add(this.label7);
            this.tabPage2.Controls.Add(this.label6);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(681, 166);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Thread";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // label11
            // 
            this.label11.Location = new System.Drawing.Point(120, 64);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(100, 16);
            this.label11.TabIndex = 5;
            this.label11.Text = "1 (Dummy Value)";
            // 
            // label10
            // 
            this.label10.Location = new System.Drawing.Point(120, 40);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(100, 16);
            this.label10.TabIndex = 4;
            this.label10.Text = "10 (Dummy Value)";
            // 
            // label9
            // 
            this.label9.Location = new System.Drawing.Point(120, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(100, 16);
            this.label9.TabIndex = 3;
            this.label9.Text = "30 (Dummy Value)";
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(16, 64);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(100, 16);
            this.label8.TabIndex = 2;
            this.label8.Text = "Block Time > 10s:";
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(16, 40);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(100, 16);
            this.label7.TabIndex = 1;
            this.label7.Text = "Block Time > 5s:";
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(16, 16);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(100, 16);
            this.label6.TabIndex = 0;
            this.label6.Text = "Block Time > 1s:";
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.label14);
            this.tabPage3.Controls.Add(this.label12);
            this.tabPage3.Controls.Add(this.label13);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(681, 166);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Template";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // label14
            // 
            this.label14.Location = new System.Drawing.Point(16, 40);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(100, 16);
            this.label14.TabIndex = 6;
            this.label14.Text = "Time Distribution:";
            // 
            // label12
            // 
            this.label12.Location = new System.Drawing.Point(120, 16);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(136, 16);
            this.label12.TabIndex = 5;
            this.label12.Text = "3000 (Dummy Value)";
            // 
            // label13
            // 
            this.label13.Location = new System.Drawing.Point(16, 16);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(100, 16);
            this.label13.TabIndex = 4;
            this.label13.Text = "Occurrence:";
            // 
            // splitterRight
            // 
            this.splitterRight.BackColor = System.Drawing.Color.WhiteSmoke;
            this.splitterRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.splitterRight.Location = new System.Drawing.Point(854, 0);
            this.splitterRight.Name = "splitterRight";
            this.splitterRight.Size = new System.Drawing.Size(7, 485);
            this.splitterRight.TabIndex = 3;
            this.splitterRight.TabStop = false;
            // 
            // splitterLeft
            // 
            this.splitterLeft.BackColor = System.Drawing.Color.WhiteSmoke;
            this.splitterLeft.Location = new System.Drawing.Point(156, 0);
            this.splitterLeft.Name = "splitterLeft";
            this.splitterLeft.Size = new System.Drawing.Size(7, 485);
            this.splitterLeft.TabIndex = 2;
            this.splitterLeft.TabStop = false;
            // 
            // panelRight
            // 
            this.panelRight.BackColor = System.Drawing.Color.WhiteSmoke;
            this.panelRight.Controls.Add(this.splitContainerStatistics);
            this.panelRight.Controls.Add(this.splitterPlaceHolder);
            this.panelRight.Dock = System.Windows.Forms.DockStyle.Right;
            this.panelRight.Location = new System.Drawing.Point(861, 0);
            this.panelRight.Name = "panelRight";
            this.panelRight.Size = new System.Drawing.Size(161, 485);
            this.panelRight.TabIndex = 1;
            // 
            // splitContainerStatistics
            // 
            this.splitContainerStatistics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerStatistics.Location = new System.Drawing.Point(0, 0);
            this.splitContainerStatistics.Margin = new System.Windows.Forms.Padding(3, 3, 20, 3);
            this.splitContainerStatistics.Name = "splitContainerStatistics";
            this.splitContainerStatistics.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerStatistics.Panel1
            // 
            this.splitContainerStatistics.Panel1.Controls.Add(this.chartTimeLine);
            this.splitContainerStatistics.Panel1.Controls.Add(this.timeline1);
            // 
            // splitContainerStatistics.Panel2
            // 
            this.splitContainerStatistics.Panel2.Controls.Add(this.propertyGridStatistics);
            this.splitContainerStatistics.Size = new System.Drawing.Size(156, 485);
            this.splitContainerStatistics.SplitterDistance = 137;
            this.splitContainerStatistics.SplitterWidth = 8;
            this.splitContainerStatistics.TabIndex = 0;
            // 
            // chartTimeLine
            // 
            this.chartTimeLine.BorderlineColor = System.Drawing.Color.Black;
            this.chartTimeLine.BorderlineDashStyle = System.Windows.Forms.DataVisualization.Charting.ChartDashStyle.Solid;
            this.chartTimeLine.CausesValidation = false;
            chartArea2.AxisX.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.False;
            chartArea2.Name = "ChartArea1";
            this.chartTimeLine.ChartAreas.Add(chartArea2);
            this.chartTimeLine.Dock = System.Windows.Forms.DockStyle.Fill;
            this.chartTimeLine.Location = new System.Drawing.Point(0, 0);
            this.chartTimeLine.Name = "chartTimeLine";
            this.chartTimeLine.Size = new System.Drawing.Size(156, 137);
            this.chartTimeLine.TabIndex = 1;
            this.chartTimeLine.Text = "Timeline";
            // 
            // propertyGridStatistics
            // 
            this.propertyGridStatistics.DisabledItemForeColor = System.Drawing.SystemColors.MenuHighlight;
            this.propertyGridStatistics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridStatistics.Location = new System.Drawing.Point(0, 0);
            this.propertyGridStatistics.Name = "propertyGridStatistics";
            this.propertyGridStatistics.Size = new System.Drawing.Size(156, 340);
            this.propertyGridStatistics.TabIndex = 0;
            // 
            // splitterPlaceHolder
            // 
            this.splitterPlaceHolder.Dock = System.Windows.Forms.DockStyle.Right;
            this.splitterPlaceHolder.Location = new System.Drawing.Point(156, 0);
            this.splitterPlaceHolder.Name = "splitterPlaceHolder";
            this.splitterPlaceHolder.Size = new System.Drawing.Size(5, 485);
            this.splitterPlaceHolder.TabIndex = 1;
            this.splitterPlaceHolder.TabStop = false;
            // 
            // panelLeft
            // 
            this.panelLeft.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(192)))));
            this.panelLeft.Controls.Add(this.treeViewDoc);
            this.panelLeft.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelLeft.Location = new System.Drawing.Point(0, 0);
            this.panelLeft.Name = "panelLeft";
            this.panelLeft.Size = new System.Drawing.Size(156, 485);
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
            treeNode2.Name = "Node0";
            treeNode2.Text = "<no doc>";
            this.treeViewDoc.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode2});
            this.treeViewDoc.ShowLines = false;
            this.treeViewDoc.Size = new System.Drawing.Size(156, 485);
            this.treeViewDoc.TabIndex = 0;
            this.treeViewDoc.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewDoc_BeforeSelect);
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
            this.statusStripMain.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel,
            this.toolStripStatusLabelFill,
            this.toolStripStatusLabel1,
            this.toolStripStatusLabelSelected,
            this.toolStripStatusLabel2});
            this.statusStripMain.Location = new System.Drawing.Point(0, 539);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.statusStripMain.Size = new System.Drawing.Size(1022, 22);
            this.statusStripMain.TabIndex = 8;
            this.statusStripMain.Text = "statusStrip1";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusLabel.Text = "Ready";
            // 
            // toolStripStatusLabelFill
            // 
            this.toolStripStatusLabelFill.Name = "toolStripStatusLabelFill";
            this.toolStripStatusLabelFill.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(920, 17);
            this.toolStripStatusLabel1.Spring = true;
            // 
            // toolStripStatusLabelSelected
            // 
            this.toolStripStatusLabelSelected.Name = "toolStripStatusLabelSelected";
            this.toolStripStatusLabelSelected.Size = new System.Drawing.Size(35, 17);
            this.toolStripStatusLabelSelected.Text = "-1 / 0";
            this.toolStripStatusLabelSelected.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(13, 17);
            this.toolStripStatusLabel2.Text = "  ";
            // 
            // fastListViewMain
            // 
            this.fastListViewMain.AllowColumnReorder = true;
            this.fastListViewMain.AlternateBackColor = System.Drawing.Color.RoyalBlue;
            this.fastListViewMain.AlternateBackColorBrush = null;
            this.fastListViewMain.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.fastListViewMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fastListViewMain.FullRowSelect = true;
            this.fastListViewMain.GridLineColor = System.Drawing.Color.Gray;
            this.fastListViewMain.GridLineColorPen = null;
            this.fastListViewMain.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.fastListViewMain.HideSelection = false;
            this.fastListViewMain.LabelWrap = false;
            this.fastListViewMain.Location = new System.Drawing.Point(0, 5);
            this.fastListViewMain.Name = "fastListViewMain";
            this.fastListViewMain.OwnerDraw = true;
            this.fastListViewMain.SelectionBackColor = System.Drawing.Color.DodgerBlue;
            this.fastListViewMain.SelectionBackColorBrush = null;
            this.fastListViewMain.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            this.fastListViewMain.SelectionForeColorBrush = null;
            this.fastListViewMain.ShowGroups = false;
            this.fastListViewMain.Size = new System.Drawing.Size(689, 277);
            this.fastListViewMain.TabIndex = 2;
            this.fastListViewMain.UseCompatibleStateImageBehavior = false;
            this.fastListViewMain.View = System.Windows.Forms.View.Details;
            this.fastListViewMain.VirtualMode = true;
            this.fastListViewMain.CacheVirtualItems += new System.Windows.Forms.CacheVirtualItemsEventHandler(this.fastListViewMain_CacheVirtualItems);
            this.fastListViewMain.DrawColumnHeader += new System.Windows.Forms.DrawListViewColumnHeaderEventHandler(this.fastListViewMain_DrawColumnHeader);
            this.fastListViewMain.DrawItem += new System.Windows.Forms.DrawListViewItemEventHandler(this.fastListViewMain_DrawItem);
            this.fastListViewMain.DrawSubItem += new System.Windows.Forms.DrawListViewSubItemEventHandler(this.fastListViewMain_DrawSubItem);
            this.fastListViewMain.RetrieveVirtualItem += new System.Windows.Forms.RetrieveVirtualItemEventHandler(this.fastListViewMain_RetrieveVirtualItem);
            this.fastListViewMain.SelectedIndexChanged += new System.EventHandler(this.fastListViewMain_SelectedIndexChanged);
            this.fastListViewMain.Resize += new System.EventHandler(this.fastListViewMain_Resize);
            // 
            // timeline1
            // 
            this.timeline1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.timeline1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.timeline1.Location = new System.Drawing.Point(0, 0);
            this.timeline1.Name = "timeline1";
            this.timeline1.Plots = ((System.Collections.Generic.IList<double>)(resources.GetObject("timeline1.Plots")));
            this.timeline1.Size = new System.Drawing.Size(156, 137);
            this.timeline1.TabIndex = 0;
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.WorkerSupportsCancellation = true;
            // 
            // MainFormListView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1022, 561);
            this.Controls.Add(this.panelMain);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.menuStripMain);
            this.Controls.Add(this.statusStripMain);
            this.DoubleBuffered = true;
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "MainFormListView";
            this.Text = "Smart Viewer";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.panelTop.ResumeLayout(false);
            this.panelTop.PerformLayout();
            this.toolStripMain.ResumeLayout(false);
            this.toolStripMain.PerformLayout();
            this.menuStripMain.ResumeLayout(false);
            this.menuStripMain.PerformLayout();
            this.panelMain.ResumeLayout(false);
            this.panelMiddle.ResumeLayout(false);
            this.splitContainerDetails.Panel1.ResumeLayout(false);
            this.splitContainerDetails.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerDetails)).EndInit();
            this.splitContainerDetails.ResumeLayout(false);
            this.tabControlDetail.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.splitContainerDetail.Panel1.ResumeLayout(false);
            this.splitContainerDetail.Panel2.ResumeLayout(false);
            this.splitContainerDetail.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerDetail)).EndInit();
            this.splitContainerDetail.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.panelRight.ResumeLayout(false);
            this.splitContainerStatistics.Panel1.ResumeLayout(false);
            this.splitContainerStatistics.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerStatistics)).EndInit();
            this.splitContainerStatistics.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.chartTimeLine)).EndInit();
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
        private System.Windows.Forms.Splitter splitterRight;
        private System.Windows.Forms.Splitter splitterLeft;
        private System.Windows.Forms.Panel panelRight;
        private System.Windows.Forms.Panel panelLeft;
        private System.Windows.Forms.TreeView treeViewDoc;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripTreeView;
        private System.Windows.Forms.ToolStripMenuItem filterToolStripMenuItemDoc;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripTextBox toolStripTextBoxPattern;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.ProgressBar progressBarMain;
        private System.Windows.Forms.ToolStripButton toolStripButtonFilter;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButtonTag;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButtonFind;
        private System.Windows.Forms.ToolStripMenuItem findPreviousToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findNextToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonCount;
        private System.Windows.Forms.ToolStripLabel toolStripLabelCount;
        private FastListView fastListViewMain;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFill;
        private System.Windows.Forms.ToolStripMenuItem filterF1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tagF2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem searchF3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem countF4ToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainerStatistics;
        private System.Windows.Forms.PropertyGrid propertyGridStatistics;
        private Timeline timeline1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelSelected;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.SplitContainer splitContainerDetails;
        private System.Windows.Forms.TabControl tabControlDetail;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.SplitContainer splitContainerDetail;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label labelProcessId;
        private System.Windows.Forms.Label labelThreadId;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelTime;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label labelLevel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelId;
        private System.Windows.Forms.TextBox textBoxText;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Splitter splitterPlaceHolder;
        private System.Windows.Forms.DataVisualization.Charting.Chart chartTimeLine;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
    }
}

