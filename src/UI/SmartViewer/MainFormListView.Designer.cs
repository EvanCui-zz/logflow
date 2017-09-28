namespace LogFlow.Viewer
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
            }

            if (disposing) this.cts?.Dispose();
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            this.panelTop = new System.Windows.Forms.Panel();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.toolStripComboBoxString = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripButtonFilter = new System.Windows.Forms.ToolStripButton();
            this.toolStripButtonGoto = new System.Windows.Forms.ToolStripButton();
            this.toolStripSplitButtonFind = new System.Windows.Forms.ToolStripSplitButton();
            this.findPreviousToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.findNextToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripButtonCount = new System.Windows.Forms.ToolStripButton();
            this.toolStripLabelCount = new System.Windows.Forms.ToolStripLabel();
            this.toolStripSeparator11 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonTag1 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator9 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonTag2 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator10 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripButtonTag3 = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.menuStripMain = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filteredOpenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.recentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.file1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.file2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterF1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gotoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.searchF3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.countF4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemTag = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.selectionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sendViaEmailToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.autoAnalyzeRulesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bug360425ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.specialPatternToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logSourcePluginsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.plainTextFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cosmosLogFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logParserPipelineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.databaseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.azureStorageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.customizedPluginToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowsEventsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hpcLogsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.boldParametersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showLevelColumnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showFilenameColumnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showActivityColumnToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.highlightLevelsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showStatisticsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fastRenderingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.detectNewLogsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.autoScrollToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.enableDataVirtualizationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.allSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.versionHistoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.feedbackToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panelMain = new System.Windows.Forms.Panel();
            this.panelMiddle = new System.Windows.Forms.Panel();
            this.splitContainerDetails = new System.Windows.Forms.SplitContainer();
            this.fastListViewMain = new LogFlow.Viewer.FastListView();
            this.contextMenuStripListView = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.filterWithTheSameThreadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterWithTheSameActivityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterAsBeginTimeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filterAsEndTimeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.indentTheThreadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.indentTheActivityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unindentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unindentTheActivityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unindentAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripMenuItemFindParent = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemFindRoot = new System.Windows.Forms.ToolStripMenuItem();
            this.findTheTimeInParentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.someOtherViewtodoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator8 = new System.Windows.Forms.ToolStripSeparator();
            this.copyToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.sendViaEmailToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.advancedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.tabPage7 = new System.Windows.Forms.TabPage();
            this.splitterRight = new System.Windows.Forms.Splitter();
            this.splitterLeft = new System.Windows.Forms.Splitter();
            this.panelRight = new System.Windows.Forms.Panel();
            this.splitContainerStatistics = new System.Windows.Forms.SplitContainer();
            this.chartTimeLine = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.propertyGridStatistics = new System.Windows.Forms.PropertyGrid();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.listViewExceptions = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.splitterPlaceHolder = new System.Windows.Forms.Splitter();
            this.panelLeft = new System.Windows.Forms.Panel();
            this.treeViewDoc = new System.Windows.Forms.TreeView();
            this.contextMenuStripTreeView = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.filterToolStripMenuItemDoc = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelFill = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelProgressUsed = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelCompress = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel5 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelSelected = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabelMemory = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.timerMemory = new System.Windows.Forms.Timer(this.components);
            this.timerDocUpdate = new System.Windows.Forms.Timer(this.components);
            this.panelTop.SuspendLayout();
            this.toolStripMain.SuspendLayout();
            this.menuStripMain.SuspendLayout();
            this.panelMain.SuspendLayout();
            this.panelMiddle.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainerDetails)).BeginInit();
            this.splitContainerDetails.Panel1.SuspendLayout();
            this.splitContainerDetails.Panel2.SuspendLayout();
            this.splitContainerDetails.SuspendLayout();
            this.contextMenuStripListView.SuspendLayout();
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
            this.tabControl1.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.tabPage5.SuspendLayout();
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
            this.toolStripComboBoxString,
            this.toolStripButtonFilter,
            this.toolStripButtonGoto,
            this.toolStripSplitButtonFind,
            this.toolStripButtonCount,
            this.toolStripLabelCount,
            this.toolStripSeparator11,
            this.toolStripButtonTag1,
            this.toolStripSeparator9,
            this.toolStripButtonTag2,
            this.toolStripSeparator10,
            this.toolStripButtonTag3,
            this.toolStripSeparator2});
            this.toolStripMain.Location = new System.Drawing.Point(0, 0);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(784, 25);
            this.toolStripMain.TabIndex = 0;
            this.toolStripMain.Text = "toolStrip1";
            // 
            // toolStripComboBoxString
            // 
            this.toolStripComboBoxString.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.toolStripComboBoxString.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.toolStripComboBoxString.Enabled = false;
            this.toolStripComboBoxString.Name = "toolStripComboBoxString";
            this.toolStripComboBoxString.Size = new System.Drawing.Size(300, 25);
            this.toolStripComboBoxString.KeyDown += new System.Windows.Forms.KeyEventHandler(this.toolStripComboBoxString_KeyDown);
            this.toolStripComboBoxString.TextChanged += new System.EventHandler(this.toolStripComboBoxString_TextChanged);
            // 
            // toolStripButtonFilter
            // 
            this.toolStripButtonFilter.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonFilter.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonFilter.Image")));
            this.toolStripButtonFilter.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonFilter.Name = "toolStripButtonFilter";
            this.toolStripButtonFilter.Size = new System.Drawing.Size(60, 22);
            this.toolStripButtonFilter.Text = "Filter (F1)";
            this.toolStripButtonFilter.ToolTipText = "Filter (F2)";
            this.toolStripButtonFilter.Click += new System.EventHandler(this.toolStripButtonFilter_Click);
            // 
            // toolStripButtonGoto
            // 
            this.toolStripButtonGoto.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonGoto.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonGoto.Image")));
            this.toolStripButtonGoto.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonGoto.Name = "toolStripButtonGoto";
            this.toolStripButtonGoto.Size = new System.Drawing.Size(60, 22);
            this.toolStripButtonGoto.Text = "Goto (F2)";
            this.toolStripButtonGoto.Click += new System.EventHandler(this.toolStripButtonGoto_Click);
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
            // toolStripSeparator11
            // 
            this.toolStripSeparator11.Name = "toolStripSeparator11";
            this.toolStripSeparator11.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonTag1
            // 
            this.toolStripButtonTag1.CheckOnClick = true;
            this.toolStripButtonTag1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonTag1.Enabled = false;
            this.toolStripButtonTag1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonTag1.Image")));
            this.toolStripButtonTag1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTag1.Name = "toolStripButtonTag1";
            this.toolStripButtonTag1.Size = new System.Drawing.Size(36, 22);
            this.toolStripButtonTag1.Text = "Tag&1";
            this.toolStripButtonTag1.Click += new System.EventHandler(this.toolStripButtonTag1_Click);
            // 
            // toolStripSeparator9
            // 
            this.toolStripSeparator9.Name = "toolStripSeparator9";
            this.toolStripSeparator9.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonTag2
            // 
            this.toolStripButtonTag2.CheckOnClick = true;
            this.toolStripButtonTag2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonTag2.Enabled = false;
            this.toolStripButtonTag2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonTag2.Image")));
            this.toolStripButtonTag2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTag2.Name = "toolStripButtonTag2";
            this.toolStripButtonTag2.Size = new System.Drawing.Size(36, 22);
            this.toolStripButtonTag2.Text = "Tag&2";
            this.toolStripButtonTag2.Click += new System.EventHandler(this.toolStripButtonTag2_Click);
            // 
            // toolStripSeparator10
            // 
            this.toolStripSeparator10.Name = "toolStripSeparator10";
            this.toolStripSeparator10.Size = new System.Drawing.Size(6, 25);
            // 
            // toolStripButtonTag3
            // 
            this.toolStripButtonTag3.CheckOnClick = true;
            this.toolStripButtonTag3.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.toolStripButtonTag3.Enabled = false;
            this.toolStripButtonTag3.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripButtonTag3.Name = "toolStripButtonTag3";
            this.toolStripButtonTag3.Size = new System.Drawing.Size(36, 22);
            this.toolStripButtonTag3.Text = "Tag&3";
            this.toolStripButtonTag3.Click += new System.EventHandler(this.toolStripButtonTag3_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // menuStripMain
            // 
            this.menuStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.searchToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.settingsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStripMain.Location = new System.Drawing.Point(0, 0);
            this.menuStripMain.Name = "menuStripMain";
            this.menuStripMain.Size = new System.Drawing.Size(1022, 24);
            this.menuStripMain.TabIndex = 1;
            this.menuStripMain.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.filteredOpenToolStripMenuItem,
            this.closeToolStripMenuItem1,
            this.recentToolStripMenuItem,
            this.toolStripSeparator3,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.openToolStripMenuItem.Text = "&Open...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // filteredOpenToolStripMenuItem
            // 
            this.filteredOpenToolStripMenuItem.Name = "filteredOpenToolStripMenuItem";
            this.filteredOpenToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.filteredOpenToolStripMenuItem.Text = "&Filtered Open...";
            this.filteredOpenToolStripMenuItem.Click += new System.EventHandler(this.filteredOpenToolStripMenuItem_Click);
            // 
            // closeToolStripMenuItem1
            // 
            this.closeToolStripMenuItem1.Name = "closeToolStripMenuItem1";
            this.closeToolStripMenuItem1.Size = new System.Drawing.Size(154, 22);
            this.closeToolStripMenuItem1.Text = "&Close";
            this.closeToolStripMenuItem1.Click += new System.EventHandler(this.closeToolStripMenuItem1_Click);
            // 
            // recentToolStripMenuItem
            // 
            this.recentToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.file1ToolStripMenuItem,
            this.file2ToolStripMenuItem});
            this.recentToolStripMenuItem.Name = "recentToolStripMenuItem";
            this.recentToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.recentToolStripMenuItem.Text = "&Recent (todo)";
            // 
            // file1ToolStripMenuItem
            // 
            this.file1ToolStripMenuItem.Name = "file1ToolStripMenuItem";
            this.file1ToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.file1ToolStripMenuItem.Text = "File 1";
            // 
            // file2ToolStripMenuItem
            // 
            this.file2ToolStripMenuItem.Name = "file2ToolStripMenuItem";
            this.file2ToolStripMenuItem.Size = new System.Drawing.Size(101, 22);
            this.file2ToolStripMenuItem.Text = "File 2";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(151, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.exitToolStripMenuItem.Text = "&Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // searchToolStripMenuItem
            // 
            this.searchToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterF1ToolStripMenuItem,
            this.gotoToolStripMenuItem,
            this.searchF3ToolStripMenuItem,
            this.countF4ToolStripMenuItem,
            this.toolStripMenuItemTag,
            this.toolStripSeparator4,
            this.selectionToolStripMenuItem});
            this.searchToolStripMenuItem.Name = "searchToolStripMenuItem";
            this.searchToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
            this.searchToolStripMenuItem.Text = "&Analyze";
            // 
            // filterF1ToolStripMenuItem
            // 
            this.filterF1ToolStripMenuItem.Name = "filterF1ToolStripMenuItem";
            this.filterF1ToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.filterF1ToolStripMenuItem.Text = "&Filter (F1)";
            // 
            // gotoToolStripMenuItem
            // 
            this.gotoToolStripMenuItem.Name = "gotoToolStripMenuItem";
            this.gotoToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.gotoToolStripMenuItem.Text = "&Goto (F2)";
            this.gotoToolStripMenuItem.Click += new System.EventHandler(this.gotoToolStripMenuItem_Click);
            // 
            // searchF3ToolStripMenuItem
            // 
            this.searchF3ToolStripMenuItem.Name = "searchF3ToolStripMenuItem";
            this.searchF3ToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.searchF3ToolStripMenuItem.Text = "&Search (F3)";
            // 
            // countF4ToolStripMenuItem
            // 
            this.countF4ToolStripMenuItem.Name = "countF4ToolStripMenuItem";
            this.countF4ToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.countF4ToolStripMenuItem.Text = "&Count (F4)";
            // 
            // toolStripMenuItemTag
            // 
            this.toolStripMenuItemTag.Name = "toolStripMenuItemTag";
            this.toolStripMenuItemTag.Size = new System.Drawing.Size(158, 22);
            this.toolStripMenuItemTag.Text = "&Tag";
            this.toolStripMenuItemTag.Click += new System.EventHandler(this.toolStripMenuItemTag_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(155, 6);
            // 
            // selectionToolStripMenuItem
            // 
            this.selectionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem,
            this.sendViaEmailToolStripMenuItem});
            this.selectionToolStripMenuItem.Name = "selectionToolStripMenuItem";
            this.selectionToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.selectionToolStripMenuItem.Text = "S&election (todo)";
            // 
            // copyToolStripMenuItem
            // 
            this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
            this.copyToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.copyToolStripMenuItem.Text = "Copy";
            // 
            // sendViaEmailToolStripMenuItem
            // 
            this.sendViaEmailToolStripMenuItem.Name = "sendViaEmailToolStripMenuItem";
            this.sendViaEmailToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.sendViaEmailToolStripMenuItem.Text = "Send via Email";
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.autoAnalyzeRulesToolStripMenuItem,
            this.logSourcePluginsToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(78, 20);
            this.toolsToolStripMenuItem.Text = "&Tool (todo)";
            // 
            // autoAnalyzeRulesToolStripMenuItem
            // 
            this.autoAnalyzeRulesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.bug360425ToolStripMenuItem,
            this.specialPatternToolStripMenuItem});
            this.autoAnalyzeRulesToolStripMenuItem.Name = "autoAnalyzeRulesToolStripMenuItem";
            this.autoAnalyzeRulesToolStripMenuItem.Size = new System.Drawing.Size(211, 22);
            this.autoAnalyzeRulesToolStripMenuItem.Text = "Auto Analyze Rules (todo)";
            // 
            // bug360425ToolStripMenuItem
            // 
            this.bug360425ToolStripMenuItem.Name = "bug360425ToolStripMenuItem";
            this.bug360425ToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.bug360425ToolStripMenuItem.Text = "Bug 360425";
            // 
            // specialPatternToolStripMenuItem
            // 
            this.specialPatternToolStripMenuItem.Name = "specialPatternToolStripMenuItem";
            this.specialPatternToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.specialPatternToolStripMenuItem.Text = "Special Pattern ";
            // 
            // logSourcePluginsToolStripMenuItem
            // 
            this.logSourcePluginsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.plainTextFileToolStripMenuItem,
            this.cosmosLogFileToolStripMenuItem,
            this.logParserPipelineToolStripMenuItem,
            this.databaseToolStripMenuItem,
            this.azureStorageToolStripMenuItem,
            this.customizedPluginToolStripMenuItem,
            this.windowsEventsToolStripMenuItem,
            this.hpcLogsToolStripMenuItem});
            this.logSourcePluginsToolStripMenuItem.Name = "logSourcePluginsToolStripMenuItem";
            this.logSourcePluginsToolStripMenuItem.Size = new System.Drawing.Size(211, 22);
            this.logSourcePluginsToolStripMenuItem.Text = "Log Source Plugins (todo)";
            // 
            // plainTextFileToolStripMenuItem
            // 
            this.plainTextFileToolStripMenuItem.Name = "plainTextFileToolStripMenuItem";
            this.plainTextFileToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.plainTextFileToolStripMenuItem.Text = "PlainText File";
            // 
            // cosmosLogFileToolStripMenuItem
            // 
            this.cosmosLogFileToolStripMenuItem.Name = "cosmosLogFileToolStripMenuItem";
            this.cosmosLogFileToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.cosmosLogFileToolStripMenuItem.Text = "Cosmos Log File";
            // 
            // logParserPipelineToolStripMenuItem
            // 
            this.logParserPipelineToolStripMenuItem.Name = "logParserPipelineToolStripMenuItem";
            this.logParserPipelineToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.logParserPipelineToolStripMenuItem.Text = "Log Parser Pipeline";
            // 
            // databaseToolStripMenuItem
            // 
            this.databaseToolStripMenuItem.Name = "databaseToolStripMenuItem";
            this.databaseToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.databaseToolStripMenuItem.Text = "Database";
            // 
            // azureStorageToolStripMenuItem
            // 
            this.azureStorageToolStripMenuItem.Name = "azureStorageToolStripMenuItem";
            this.azureStorageToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.azureStorageToolStripMenuItem.Text = "Azure Storage";
            // 
            // customizedPluginToolStripMenuItem
            // 
            this.customizedPluginToolStripMenuItem.Name = "customizedPluginToolStripMenuItem";
            this.customizedPluginToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.customizedPluginToolStripMenuItem.Text = "Customized Source";
            // 
            // windowsEventsToolStripMenuItem
            // 
            this.windowsEventsToolStripMenuItem.Name = "windowsEventsToolStripMenuItem";
            this.windowsEventsToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.windowsEventsToolStripMenuItem.Text = "Windows Events";
            // 
            // hpcLogsToolStripMenuItem
            // 
            this.hpcLogsToolStripMenuItem.Name = "hpcLogsToolStripMenuItem";
            this.hpcLogsToolStripMenuItem.Size = new System.Drawing.Size(176, 22);
            this.hpcLogsToolStripMenuItem.Text = "Hpc Logs";
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.boldParametersToolStripMenuItem,
            this.showLevelColumnToolStripMenuItem,
            this.showFilenameColumnToolStripMenuItem,
            this.showActivityColumnToolStripMenuItem,
            this.highlightLevelsToolStripMenuItem,
            this.showStatisticsToolStripMenuItem,
            this.fastRenderingToolStripMenuItem,
            this.detectNewLogsToolStripMenuItem,
            this.autoScrollToolStripMenuItem,
            this.enableDataVirtualizationToolStripMenuItem,
            this.toolStripSeparator5,
            this.allSettingsToolStripMenuItem});
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.settingsToolStripMenuItem.Text = "&Settings";
            // 
            // boldParametersToolStripMenuItem
            // 
            this.boldParametersToolStripMenuItem.Checked = true;
            this.boldParametersToolStripMenuItem.CheckOnClick = true;
            this.boldParametersToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.boldParametersToolStripMenuItem.Name = "boldParametersToolStripMenuItem";
            this.boldParametersToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.boldParametersToolStripMenuItem.Text = "&Bold Parameters";
            this.boldParametersToolStripMenuItem.CheckedChanged += new System.EventHandler(this.boldParametersToolStripMenuItem_CheckedChanged);
            // 
            // showLevelColumnToolStripMenuItem
            // 
            this.showLevelColumnToolStripMenuItem.CheckOnClick = true;
            this.showLevelColumnToolStripMenuItem.Name = "showLevelColumnToolStripMenuItem";
            this.showLevelColumnToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.showLevelColumnToolStripMenuItem.Text = "Show &Level Column";
            this.showLevelColumnToolStripMenuItem.CheckedChanged += new System.EventHandler(this.showLevelColumnToolStripMenuItem_CheckedChanged);
            // 
            // showFilenameColumnToolStripMenuItem
            // 
            this.showFilenameColumnToolStripMenuItem.CheckOnClick = true;
            this.showFilenameColumnToolStripMenuItem.Name = "showFilenameColumnToolStripMenuItem";
            this.showFilenameColumnToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.showFilenameColumnToolStripMenuItem.Text = "Show &Filename Column";
            this.showFilenameColumnToolStripMenuItem.CheckedChanged += new System.EventHandler(this.showFilenameColumnToolStripMenuItem_CheckedChanged);
            // 
            // showActivityColumnToolStripMenuItem
            // 
            this.showActivityColumnToolStripMenuItem.CheckOnClick = true;
            this.showActivityColumnToolStripMenuItem.Name = "showActivityColumnToolStripMenuItem";
            this.showActivityColumnToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.showActivityColumnToolStripMenuItem.Text = "Show &Activity Column";
            this.showActivityColumnToolStripMenuItem.CheckedChanged += new System.EventHandler(this.showActivityColumnToolStripMenuItem_CheckedChanged);
            // 
            // highlightLevelsToolStripMenuItem
            // 
            this.highlightLevelsToolStripMenuItem.Checked = true;
            this.highlightLevelsToolStripMenuItem.CheckOnClick = true;
            this.highlightLevelsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.highlightLevelsToolStripMenuItem.Name = "highlightLevelsToolStripMenuItem";
            this.highlightLevelsToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.highlightLevelsToolStripMenuItem.Text = "&Highlight Levels";
            this.highlightLevelsToolStripMenuItem.CheckedChanged += new System.EventHandler(this.highlightLevelsToolStripMenuItem_CheckedChanged);
            // 
            // showStatisticsToolStripMenuItem
            // 
            this.showStatisticsToolStripMenuItem.Checked = true;
            this.showStatisticsToolStripMenuItem.CheckOnClick = true;
            this.showStatisticsToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.showStatisticsToolStripMenuItem.Name = "showStatisticsToolStripMenuItem";
            this.showStatisticsToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.showStatisticsToolStripMenuItem.Text = "Show &Statistics";
            this.showStatisticsToolStripMenuItem.CheckedChanged += new System.EventHandler(this.showStatisticsToolStripMenuItem_CheckedChanged);
            // 
            // fastRenderingToolStripMenuItem
            // 
            this.fastRenderingToolStripMenuItem.CheckOnClick = true;
            this.fastRenderingToolStripMenuItem.Name = "fastRenderingToolStripMenuItem";
            this.fastRenderingToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.fastRenderingToolStripMenuItem.Text = "Fast &Rendering";
            this.fastRenderingToolStripMenuItem.CheckedChanged += new System.EventHandler(this.fastRenderingToolStripMenuItem_CheckedChanged);
            // 
            // detectNewLogsToolStripMenuItem
            // 
            this.detectNewLogsToolStripMenuItem.CheckOnClick = true;
            this.detectNewLogsToolStripMenuItem.Name = "detectNewLogsToolStripMenuItem";
            this.detectNewLogsToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.detectNewLogsToolStripMenuItem.Text = "Auto &Load New Logs (todo)";
            this.detectNewLogsToolStripMenuItem.CheckedChanged += new System.EventHandler(this.detectNewLogsToolStripMenuItem_CheckedChanged);
            // 
            // autoScrollToolStripMenuItem
            // 
            this.autoScrollToolStripMenuItem.CheckOnClick = true;
            this.autoScrollToolStripMenuItem.Name = "autoScrollToolStripMenuItem";
            this.autoScrollToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.autoScrollToolStripMenuItem.Text = "Auto S&croll (todo)";
            this.autoScrollToolStripMenuItem.CheckedChanged += new System.EventHandler(this.autoScrollToolStripMenuItem_CheckedChanged);
            // 
            // enableDataVirtualizationToolStripMenuItem
            // 
            this.enableDataVirtualizationToolStripMenuItem.CheckOnClick = true;
            this.enableDataVirtualizationToolStripMenuItem.Name = "enableDataVirtualizationToolStripMenuItem";
            this.enableDataVirtualizationToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.enableDataVirtualizationToolStripMenuItem.Text = "&Data Virtualization (todo)";
            this.enableDataVirtualizationToolStripMenuItem.CheckedChanged += new System.EventHandler(this.enableDataVirtualizationToolStripMenuItem_CheckedChanged);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(217, 6);
            // 
            // allSettingsToolStripMenuItem
            // 
            this.allSettingsToolStripMenuItem.Name = "allSettingsToolStripMenuItem";
            this.allSettingsToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.allSettingsToolStripMenuItem.Text = "All S&ettings...";
            this.allSettingsToolStripMenuItem.Click += new System.EventHandler(this.allSettingsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.contentToolStripMenuItem,
            this.versionHistoryToolStripMenuItem,
            this.feedbackToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "&Help";
            // 
            // contentToolStripMenuItem
            // 
            this.contentToolStripMenuItem.Name = "contentToolStripMenuItem";
            this.contentToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.contentToolStripMenuItem.Text = "&Content (todo)";
            // 
            // versionHistoryToolStripMenuItem
            // 
            this.versionHistoryToolStripMenuItem.Name = "versionHistoryToolStripMenuItem";
            this.versionHistoryToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.versionHistoryToolStripMenuItem.Text = "&Version History";
            this.versionHistoryToolStripMenuItem.Click += new System.EventHandler(this.versionHistoryToolStripMenuItem_Click);
            // 
            // feedbackToolStripMenuItem
            // 
            this.feedbackToolStripMenuItem.Name = "feedbackToolStripMenuItem";
            this.feedbackToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.feedbackToolStripMenuItem.Text = "&Feedback (todo)";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.aboutToolStripMenuItem.Text = "&About (todo)";
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
            // fastListViewMain
            // 
            this.fastListViewMain.AllowColumnReorder = true;
            this.fastListViewMain.AlternateBackColor = System.Drawing.Color.RoyalBlue;
            this.fastListViewMain.AlternateBackColorBrush = null;
            this.fastListViewMain.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.fastListViewMain.ContextMenuStrip = this.contextMenuStripListView;
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
            this.fastListViewMain.ColumnWidthChanged += new System.Windows.Forms.ColumnWidthChangedEventHandler(this.fastListViewMain_ColumnWidthChanged);
            this.fastListViewMain.DrawColumnHeader += new System.Windows.Forms.DrawListViewColumnHeaderEventHandler(this.fastListViewMain_DrawColumnHeader);
            this.fastListViewMain.DrawItem += new System.Windows.Forms.DrawListViewItemEventHandler(this.fastListViewMain_DrawItem);
            this.fastListViewMain.DrawSubItem += new System.Windows.Forms.DrawListViewSubItemEventHandler(this.fastListViewMain_DrawSubItem);
            this.fastListViewMain.RetrieveVirtualItem += new System.Windows.Forms.RetrieveVirtualItemEventHandler(this.fastListViewMain_RetrieveVirtualItem);
            this.fastListViewMain.SelectedIndexChanged += new System.EventHandler(this.fastListViewMain_SelectedIndexChanged);
            this.fastListViewMain.Resize += new System.EventHandler(this.fastListViewMain_Resize);
            // 
            // contextMenuStripListView
            // 
            this.contextMenuStripListView.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.filterWithTheSameThreadToolStripMenuItem,
            this.filterWithTheSameActivityToolStripMenuItem,
            this.filterAsBeginTimeToolStripMenuItem,
            this.filterAsEndTimeToolStripMenuItem,
            this.indentTheThreadToolStripMenuItem,
            this.indentTheActivityToolStripMenuItem,
            this.unindentToolStripMenuItem,
            this.unindentTheActivityToolStripMenuItem,
            this.unindentAllToolStripMenuItem,
            this.toolStripSeparator7,
            this.toolStripMenuItemFindParent,
            this.toolStripMenuItemFindRoot,
            this.findTheTimeInParentToolStripMenuItem,
            this.toolStripSeparator8,
            this.copyToolStripMenuItem1,
            this.sendViaEmailToolStripMenuItem1,
            this.toolStripSeparator6,
            this.advancedToolStripMenuItem});
            this.contextMenuStripListView.Name = "contextMenuStripListView";
            this.contextMenuStripListView.Size = new System.Drawing.Size(222, 352);
            // 
            // filterWithTheSameThreadToolStripMenuItem
            // 
            this.filterWithTheSameThreadToolStripMenuItem.Name = "filterWithTheSameThreadToolStripMenuItem";
            this.filterWithTheSameThreadToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.filterWithTheSameThreadToolStripMenuItem.Text = "Filter with the Same Thread";
            this.filterWithTheSameThreadToolStripMenuItem.Click += new System.EventHandler(this.filterWithTheSameThreadToolStripMenuItem_Click);
            // 
            // filterWithTheSameActivityToolStripMenuItem
            // 
            this.filterWithTheSameActivityToolStripMenuItem.Name = "filterWithTheSameActivityToolStripMenuItem";
            this.filterWithTheSameActivityToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.filterWithTheSameActivityToolStripMenuItem.Text = "Filter with the Same Activity";
            this.filterWithTheSameActivityToolStripMenuItem.Click += new System.EventHandler(this.filterWithTheSameActivityToolStripMenuItem_Click);
            // 
            // filterAsBeginTimeToolStripMenuItem
            // 
            this.filterAsBeginTimeToolStripMenuItem.Name = "filterAsBeginTimeToolStripMenuItem";
            this.filterAsBeginTimeToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.filterAsBeginTimeToolStripMenuItem.Text = "Filter as Start";
            this.filterAsBeginTimeToolStripMenuItem.Click += new System.EventHandler(this.filterAsStartToolStripMenuItem_Click);
            // 
            // filterAsEndTimeToolStripMenuItem
            // 
            this.filterAsEndTimeToolStripMenuItem.Name = "filterAsEndTimeToolStripMenuItem";
            this.filterAsEndTimeToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.filterAsEndTimeToolStripMenuItem.Text = "Filter as End";
            this.filterAsEndTimeToolStripMenuItem.Click += new System.EventHandler(this.filterAsEndToolStripMenuItem_Click);
            // 
            // indentTheThreadToolStripMenuItem
            // 
            this.indentTheThreadToolStripMenuItem.Name = "indentTheThreadToolStripMenuItem";
            this.indentTheThreadToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.indentTheThreadToolStripMenuItem.Text = "Indent the Thread";
            this.indentTheThreadToolStripMenuItem.Click += new System.EventHandler(this.indentTheThreadToolStripMenuItem_Click);
            // 
            // indentTheActivityToolStripMenuItem
            // 
            this.indentTheActivityToolStripMenuItem.Name = "indentTheActivityToolStripMenuItem";
            this.indentTheActivityToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.indentTheActivityToolStripMenuItem.Text = "Indent the Activity";
            this.indentTheActivityToolStripMenuItem.Click += new System.EventHandler(this.indentTheActivityToolStripMenuItem_Click);
            // 
            // unindentToolStripMenuItem
            // 
            this.unindentToolStripMenuItem.Name = "unindentToolStripMenuItem";
            this.unindentToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.unindentToolStripMenuItem.Text = "Unindent the Thread";
            this.unindentToolStripMenuItem.Click += new System.EventHandler(this.unindentToolStripMenuItem_Click);
            // 
            // unindentTheActivityToolStripMenuItem
            // 
            this.unindentTheActivityToolStripMenuItem.Name = "unindentTheActivityToolStripMenuItem";
            this.unindentTheActivityToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.unindentTheActivityToolStripMenuItem.Text = "Unindent the Activity";
            this.unindentTheActivityToolStripMenuItem.Click += new System.EventHandler(this.unindentTheActivityToolStripMenuItem_Click);
            // 
            // unindentAllToolStripMenuItem
            // 
            this.unindentAllToolStripMenuItem.Name = "unindentAllToolStripMenuItem";
            this.unindentAllToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.unindentAllToolStripMenuItem.Text = "Unindent All";
            this.unindentAllToolStripMenuItem.Click += new System.EventHandler(this.unindentAllToolStripMenuItem_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(218, 6);
            // 
            // toolStripMenuItemFindParent
            // 
            this.toolStripMenuItemFindParent.Name = "toolStripMenuItemFindParent";
            this.toolStripMenuItemFindParent.Size = new System.Drawing.Size(221, 22);
            this.toolStripMenuItemFindParent.Text = "Find the Line in Parent";
            this.toolStripMenuItemFindParent.Click += new System.EventHandler(this.ToolStripMenuItemFindParent_Click);
            // 
            // toolStripMenuItemFindRoot
            // 
            this.toolStripMenuItemFindRoot.Name = "toolStripMenuItemFindRoot";
            this.toolStripMenuItemFindRoot.Size = new System.Drawing.Size(221, 22);
            this.toolStripMenuItemFindRoot.Text = "Find the Line in Root";
            this.toolStripMenuItemFindRoot.Click += new System.EventHandler(this.ToolStripMenuItemFindRoot_Click);
            // 
            // findTheTimeInParentToolStripMenuItem
            // 
            this.findTheTimeInParentToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.someOtherViewtodoToolStripMenuItem});
            this.findTheTimeInParentToolStripMenuItem.Name = "findTheTimeInParentToolStripMenuItem";
            this.findTheTimeInParentToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.findTheTimeInParentToolStripMenuItem.Text = "Find the Time in";
            // 
            // someOtherViewtodoToolStripMenuItem
            // 
            this.someOtherViewtodoToolStripMenuItem.Name = "someOtherViewtodoToolStripMenuItem";
            this.someOtherViewtodoToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.someOtherViewtodoToolStripMenuItem.Text = "Some other view (todo)";
            // 
            // toolStripSeparator8
            // 
            this.toolStripSeparator8.Name = "toolStripSeparator8";
            this.toolStripSeparator8.Size = new System.Drawing.Size(218, 6);
            // 
            // copyToolStripMenuItem1
            // 
            this.copyToolStripMenuItem1.Name = "copyToolStripMenuItem1";
            this.copyToolStripMenuItem1.Size = new System.Drawing.Size(221, 22);
            this.copyToolStripMenuItem1.Text = "Copy";
            this.copyToolStripMenuItem1.Click += new System.EventHandler(this.copyToolStripMenuItem1_Click);
            // 
            // sendViaEmailToolStripMenuItem1
            // 
            this.sendViaEmailToolStripMenuItem1.Enabled = false;
            this.sendViaEmailToolStripMenuItem1.Name = "sendViaEmailToolStripMenuItem1";
            this.sendViaEmailToolStripMenuItem1.Size = new System.Drawing.Size(221, 22);
            this.sendViaEmailToolStripMenuItem1.Text = "Send via Email";
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(218, 6);
            // 
            // advancedToolStripMenuItem
            // 
            this.advancedToolStripMenuItem.Enabled = false;
            this.advancedToolStripMenuItem.Name = "advancedToolStripMenuItem";
            this.advancedToolStripMenuItem.Size = new System.Drawing.Size(221, 22);
            this.advancedToolStripMenuItem.Text = "Advanced Operations";
            // 
            // progressBarMain
            // 
            this.progressBarMain.Dock = System.Windows.Forms.DockStyle.Top;
            this.progressBarMain.Location = new System.Drawing.Point(0, 0);
            this.progressBarMain.Name = "progressBarMain";
            this.progressBarMain.Size = new System.Drawing.Size(689, 5);
            this.progressBarMain.TabIndex = 1;
            this.progressBarMain.Visible = false;
            // 
            // tabControlDetail
            // 
            this.tabControlDetail.Controls.Add(this.tabPage1);
            this.tabControlDetail.Controls.Add(this.tabPage2);
            this.tabControlDetail.Controls.Add(this.tabPage3);
            this.tabControlDetail.Controls.Add(this.tabPage6);
            this.tabControlDetail.Controls.Add(this.tabPage7);
            this.tabControlDetail.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlDetail.Location = new System.Drawing.Point(0, 0);
            this.tabControlDetail.Name = "tabControlDetail";
            this.tabControlDetail.SelectedIndex = 0;
            this.tabControlDetail.Size = new System.Drawing.Size(689, 195);
            this.tabControlDetail.TabIndex = 3;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.White;
            this.tabPage1.Controls.Add(this.splitContainerDetail);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(681, 169);
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
            this.splitContainerDetail.Size = new System.Drawing.Size(675, 163);
            this.splitContainerDetail.SplitterDistance = 226;
            this.splitContainerDetail.TabIndex = 4;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Location = new System.Drawing.Point(216, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(2, 144);
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
            this.textBoxText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxText.Size = new System.Drawing.Size(429, 135);
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
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(681, 169);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Thread (todo)";
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
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(681, 169);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Template (todo)";
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
            // tabPage6
            // 
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Size = new System.Drawing.Size(681, 169);
            this.tabPage6.TabIndex = 3;
            this.tabPage6.Text = "Parent Preview (todo)";
            this.tabPage6.UseVisualStyleBackColor = true;
            // 
            // tabPage7
            // 
            this.tabPage7.Location = new System.Drawing.Point(4, 22);
            this.tabPage7.Name = "tabPage7";
            this.tabPage7.Size = new System.Drawing.Size(681, 169);
            this.tabPage7.TabIndex = 4;
            this.tabPage7.Text = "Root Preview (todo)";
            this.tabPage7.UseVisualStyleBackColor = true;
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
            this.splitContainerStatistics.CausesValidation = false;
            this.splitContainerStatistics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerStatistics.Location = new System.Drawing.Point(0, 0);
            this.splitContainerStatistics.Margin = new System.Windows.Forms.Padding(3, 3, 20, 3);
            this.splitContainerStatistics.Name = "splitContainerStatistics";
            this.splitContainerStatistics.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerStatistics.Panel1
            // 
            this.splitContainerStatistics.Panel1.Controls.Add(this.chartTimeLine);
            // 
            // splitContainerStatistics.Panel2
            // 
            this.splitContainerStatistics.Panel2.Controls.Add(this.tabControl1);
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
            chartArea1.AxisX.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.False;
            chartArea1.Name = "ChartArea1";
            this.chartTimeLine.ChartAreas.Add(chartArea1);
            this.chartTimeLine.Dock = System.Windows.Forms.DockStyle.Fill;
            this.chartTimeLine.Location = new System.Drawing.Point(0, 0);
            this.chartTimeLine.Name = "chartTimeLine";
            this.chartTimeLine.Size = new System.Drawing.Size(156, 137);
            this.chartTimeLine.TabIndex = 4;
            this.chartTimeLine.Text = "Timeline";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(156, 340);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.propertyGridStatistics);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(148, 314);
            this.tabPage4.TabIndex = 0;
            this.tabPage4.Text = "Statistics";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // propertyGridStatistics
            // 
            this.propertyGridStatistics.DisabledItemForeColor = System.Drawing.SystemColors.ControlText;
            this.propertyGridStatistics.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridStatistics.HelpVisible = false;
            this.propertyGridStatistics.LineColor = System.Drawing.SystemColors.ControlDark;
            this.propertyGridStatistics.Location = new System.Drawing.Point(3, 3);
            this.propertyGridStatistics.Name = "propertyGridStatistics";
            this.propertyGridStatistics.Size = new System.Drawing.Size(142, 308);
            this.propertyGridStatistics.TabIndex = 5;
            this.propertyGridStatistics.ToolbarVisible = false;
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.listViewExceptions);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(148, 314);
            this.tabPage5.TabIndex = 1;
            this.tabPage5.Text = "Exceptions";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // listViewExceptions
            // 
            this.listViewExceptions.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listViewExceptions.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewExceptions.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewExceptions.Location = new System.Drawing.Point(3, 3);
            this.listViewExceptions.Name = "listViewExceptions";
            this.listViewExceptions.Size = new System.Drawing.Size(142, 308);
            this.listViewExceptions.TabIndex = 0;
            this.listViewExceptions.UseCompatibleStateImageBehavior = false;
            this.listViewExceptions.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "ExceptionType";
            this.columnHeader1.Width = 90;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Count";
            this.columnHeader2.Width = 50;
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
            this.treeViewDoc.ShowLines = false;
            this.treeViewDoc.Size = new System.Drawing.Size(156, 485);
            this.treeViewDoc.TabIndex = 1;
            this.treeViewDoc.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewDoc_BeforeSelect);
            this.treeViewDoc.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewDoc_AfterSelect);
            // 
            // contextMenuStripTreeView
            // 
            this.contextMenuStripTreeView.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem1,
            this.filterToolStripMenuItemDoc,
            this.toolStripSeparator1,
            this.closeToolStripMenuItem});
            this.contextMenuStripTreeView.Name = "contextMenuStripTreeView";
            this.contextMenuStripTreeView.Size = new System.Drawing.Size(155, 76);
            // 
            // openToolStripMenuItem1
            // 
            this.openToolStripMenuItem1.Name = "openToolStripMenuItem1";
            this.openToolStripMenuItem1.Size = new System.Drawing.Size(154, 22);
            this.openToolStripMenuItem1.Text = "Open...";
            this.openToolStripMenuItem1.Click += new System.EventHandler(this.openToolStripMenuItem1_Click);
            // 
            // filterToolStripMenuItemDoc
            // 
            this.filterToolStripMenuItemDoc.Name = "filterToolStripMenuItemDoc";
            this.filterToolStripMenuItemDoc.Size = new System.Drawing.Size(154, 22);
            this.filterToolStripMenuItemDoc.Text = "Filtered Open...";
            this.filterToolStripMenuItemDoc.Click += new System.EventHandler(this.filterToolStripMenuItemDoc_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(151, 6);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(154, 22);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // statusStripMain
            // 
            this.statusStripMain.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.statusStripMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel,
            this.toolStripStatusLabelFill,
            this.toolStripStatusLabelProgressUsed,
            this.toolStripStatusLabelCompress,
            this.toolStripStatusLabel5,
            this.toolStripStatusLabelSelected,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabelMemory,
            this.toolStripStatusLabel3,
            this.toolStripStatusLabel4});
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
            // toolStripStatusLabelProgressUsed
            // 
            this.toolStripStatusLabelProgressUsed.Name = "toolStripStatusLabelProgressUsed";
            this.toolStripStatusLabelProgressUsed.Size = new System.Drawing.Size(771, 17);
            this.toolStripStatusLabelProgressUsed.Spring = true;
            // 
            // toolStripStatusLabelCompress
            // 
            this.toolStripStatusLabelCompress.Name = "toolStripStatusLabelCompress";
            this.toolStripStatusLabelCompress.Size = new System.Drawing.Size(92, 17);
            this.toolStripStatusLabelCompress.Text = "Tier1: 0 | Tier2: 0";
            // 
            // toolStripStatusLabel5
            // 
            this.toolStripStatusLabel5.AutoSize = false;
            this.toolStripStatusLabel5.Name = "toolStripStatusLabel5";
            this.toolStripStatusLabel5.Size = new System.Drawing.Size(20, 17);
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
            // toolStripStatusLabelMemory
            // 
            this.toolStripStatusLabelMemory.Name = "toolStripStatusLabelMemory";
            this.toolStripStatusLabelMemory.Size = new System.Drawing.Size(24, 17);
            this.toolStripStatusLabelMemory.Text = "0M";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.AutoSize = false;
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(13, 17);
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.None;
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(0, 17);
            this.toolStripStatusLabel4.Text = "toolStripStatusLabel4";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "Cosmos files|*.bin|Log files|*.log|Text files|*.txt|All files|*.*";
            this.openFileDialog1.Multiselect = true;
            this.openFileDialog1.Title = "Select the log file";
            // 
            // timerMemory
            // 
            this.timerMemory.Interval = 1000;
            this.timerMemory.Tick += new System.EventHandler(this.timerMemory_Tick);
            // 
            // timerDocUpdate
            // 
            this.timerDocUpdate.Interval = 2000;
            this.timerDocUpdate.Tick += new System.EventHandler(this.timerDocUpdate_Tick);
            // 
            // MainFormListView
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1022, 561);
            this.Controls.Add(this.panelMain);
            this.Controls.Add(this.panelTop);
            this.Controls.Add(this.menuStripMain);
            this.Controls.Add(this.statusStripMain);
            this.DoubleBuffered = true;
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStripMain;
            this.Name = "MainFormListView";
            this.Text = "LogFlow";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainFormListView_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainFormListView_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainFormListView_DragEnter);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainFormListView_KeyDown);
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
            this.contextMenuStripListView.ResumeLayout(false);
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
            this.tabControl1.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
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
        private System.Windows.Forms.StatusStrip statusStripMain;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.ProgressBar progressBarMain;
        private System.Windows.Forms.ToolStripButton toolStripButtonFilter;
        private System.Windows.Forms.ToolStripSplitButton toolStripSplitButtonFind;
        private System.Windows.Forms.ToolStripMenuItem findPreviousToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem findNextToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonCount;
        private System.Windows.Forms.ToolStripLabel toolStripLabelCount;
        private FastListView fastListViewMain;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelFill;
        private System.Windows.Forms.ToolStripMenuItem filterF1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemTag;
        private System.Windows.Forms.ToolStripMenuItem searchF3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem countF4ToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainerStatistics;
        private System.Windows.Forms.PropertyGrid propertyGridStatistics;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelProgressUsed;
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
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem autoAnalyzeRulesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem boldParametersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showLevelColumnToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem highlightLevelsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showStatisticsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fastRenderingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bug360425ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem specialPatternToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logSourcePluginsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem recentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem file1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem file2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem plainTextFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cosmosLogFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem logParserPipelineToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem databaseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem azureStorageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem customizedPluginToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filteredOpenToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem selectionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sendViaEmailToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowsEventsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hpcLogsToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelMemory;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ToolStripMenuItem allSettingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem contentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem versionHistoryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem feedbackToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripListView;
        private System.Windows.Forms.ToolStripMenuItem filterWithTheSameThreadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filterWithTheSameActivityToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem indentTheThreadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem indentTheActivityToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unindentToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFindParent;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemFindRoot;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator8;
        private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem sendViaEmailToolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
        private System.Windows.Forms.ToolStripMenuItem advancedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unindentTheActivityToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unindentAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox toolStripComboBoxString;
        private System.Windows.Forms.ToolStripMenuItem detectNewLogsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem autoScrollToolStripMenuItem;
        private System.Windows.Forms.Timer timerMemory;
        private System.Windows.Forms.ToolStripMenuItem showFilenameColumnToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem enableDataVirtualizationToolStripMenuItem;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.ListView listViewExceptions;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ToolStripMenuItem filterAsBeginTimeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem filterAsEndTimeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showActivityColumnToolStripMenuItem;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.TabPage tabPage7;
        private System.Windows.Forms.ToolStripButton toolStripButtonTag1;
        private System.Windows.Forms.ToolStripButton toolStripButtonTag2;
        private System.Windows.Forms.ToolStripButton toolStripButtonTag3;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator9;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator10;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator11;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelCompress;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel5;
        private System.Windows.Forms.ToolStripMenuItem findTheTimeInParentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem someOtherViewtodoToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton toolStripButtonGoto;
        private System.Windows.Forms.ToolStripMenuItem gotoToolStripMenuItem;
        private System.Windows.Forms.Timer timerDocUpdate;
    }
}

