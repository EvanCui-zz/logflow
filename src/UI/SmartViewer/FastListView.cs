using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SmartViewer
{
    public partial class FastListView : ListView
    {
        public FastListView()
        {
            InitializeComponent();
            this.DoubleBuffered = true;
            this.BoldFont = new Font(this.NormalFont, FontStyle.Bold);
            this.ForeColorBrush = new SolidBrush(this.ForeColor);
            this.Columns.Add(new ColumnHeader() { Name = "WorkaroundForDrawItemBug", Text = "", Width = 0 });
        }

        public void SetHeight(int height)
        {
            ImageList imgList = new ImageList();
            imgList.ImageSize = new Size(1, height);
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
        public Color SelectionForeColor{ get; set; }
        public Brush SelectionForeColorBrush { get; set; }
        
        [Browsable(true)]
        public Color SelectionBackColor{ get; set; }
        public Brush SelectionBackColorBrush { get; set; }

        [Browsable(true)]
        public Color AlternateBackColor { get; set; }
        public Brush AlternateBackColorBrush { get; set; }

        [Browsable(true)]
        public Color GridLineColor { get; set; }
        public Pen GridLineColorPen { get; set; }


        public Font BoldFont { get; private set; }
        public Font NormalFont { get { return this.Font; } }
    }
}
