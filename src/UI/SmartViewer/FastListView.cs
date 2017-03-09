using System;
using System.Security.Cryptography.X509Certificates;
using LogFlow.Viewer.Properties;

namespace LogFlow.Viewer
{
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Drawing;
    using System.Windows.Forms;

    public partial class FastListView
    {
        public FastListView()
        {
            InitializeComponent();
            //   this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);
            this.DoubleBuffered = true;
            this.BoldFont = new Font(this.NormalFont, FontStyle.Bold);
            this.ForeColorBrush = new SolidBrush(this.ForeColor);
        }

        public void SetHeight(int height)
        {
            var imgList = new ImageList { ImageSize = new Size(1, height) };
            this.SmallImageList = imgList;
        }

        public void ReloadSetting()
        {
            this.levelBrushes?.ForEach(b => b.Dispose());
            this.levelBrushes = null;
            this.AllowColumnReorder = Settings.Default.Behavior_AllowColumnReorder;
        }

        public IList<Brush> LevelBrushes
        {
            get
            {
                return this.levelBrushes = this.levelBrushes ?? new List<Brush>()
                {
                    new SolidBrush(Settings.Default.Display_LevelColors.Critical),
                    new SolidBrush(Settings.Default.Display_LevelColors.Error),
                    new SolidBrush(Settings.Default.Display_LevelColors.Warning),
                    new SolidBrush(Settings.Default.Display_LevelColors.Info),
                    new SolidBrush(Settings.Default.Display_LevelColors.Verbose),
                    new SolidBrush(Settings.Default.Display_LevelColors.Detail),
                };
            }
        }

        private List<Brush> levelBrushes;

        public Brush ForeColorBrush { get; private set; }

        [Browsable(true)]
        public Color SelectionForeColor { get; set; }
        public Brush SelectionForeColorBrush { get; set; }

        [Browsable(true)]
        public Color SelectionBackColor { get; set; }
        public Brush SelectionBackColorBrush { get; set; }

        [Browsable(true)]
        public Color AlternateBackColor { get; set; }
        public Brush AlternateBackColorBrush { get; set; }

        [Browsable(true)]
        public Color GridLineColor { get; set; }
        public Pen GridLineColorPen { get; set; }

        public Font BoldFont { get; }
        public Font NormalFont => this.Font;
    }
}
