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
    public partial class Timeline : UserControl
    {
        public Timeline()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.ResizeRedraw, true);
        }

        public IList<double> Plots { get; set; } = new List<double>(10) { 2, 4, 14, 15, 18, 22, 29, 40, 44, 45 };

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
//            e.Graphics.Clear(Color.Violet);
            //e.Graphics.DrawLine(Pens.Blue, e.ClipRectangle.Location, new Point(e.ClipRectangle.X + e.ClipRectangle.Width, e.ClipRectangle.Y + e.ClipRectangle.Height));
            //return;
            Point start = new Point(0, this.Height);
            double plotsRange = this.Plots[this.Plots.Count - 1] - this.Plots[0];

            for (int i = 1; i < this.Plots.Count; i++)
            {
                double y = (this.Plots[i] - this.Plots[0]) * this.Height / plotsRange;
                int x = this.Width * i / this.Plots.Count;

                Point next = new Point(x, this.Height - (int)y);
                e.Graphics.DrawLine(Pens.DarkGoldenrod, start, next);
                start = next;
            }
        }
    }
}
