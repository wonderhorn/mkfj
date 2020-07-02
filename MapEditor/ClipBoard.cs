using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    public partial class ClipBoard : Form
    {
        private MapData data;
        private PictureBox picbox;
        public ClipBoard()
        {
            InitializeComponent();
            data = new MapData();
            picbox = this.pictureBox1;
            this.panel1.Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right | AnchorStyles.Top;
            picbox.Parent = this.panel1;
            this.panel1.Parent = this;
            this.AutoScroll = true;
            this.panel1.AutoScroll = true;
        }

        public void setChips(MapData src, int x, int y, int w, int h)
        {
            data = new MapData();
            data.Width = w;
            data.Height = h;
            picbox.Size = new Size(64 * w, 64 * h);
            for (int j = y; j < y + h; j++)
            {
                for (int i = x; i < x + w; i++) 
                {
                    Point p = new Point((i) * 64, (j) * 64);
                    Point newp = new Point((i - x) * 64, (j - y) * 64);
                    Mapchip c = src[p];
                    if (c != null) 
                    {
                        data[newp] = c;
                    }
                }
            }
            Refresh();
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            data.paint(e.Graphics);
        }
    }
}
