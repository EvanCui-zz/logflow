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
            var imgList = new ImageList {ImageSize = new Size(1, height)};
            this.SmallImageList = imgList;
        }

        [Browsable(true)]
        public IList<Brush> LevelBrushes { get; } = new List<Brush>()
        {
            new SolidBrush(Color.FromArgb(255, 0, 0)),
            new SolidBrush(Color.FromArgb(200, 0, 0)),
            new SolidBrush(Color.FromArgb(200, 180, 0)),
        };

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
