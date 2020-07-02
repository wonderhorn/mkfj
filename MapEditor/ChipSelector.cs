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
    public partial class ChipSelector : Form
    {
        RectCursor cursor;
        private String img_name;
        private int chip_size;
        public ChipSelector(String img_name, int chip_size = 64)
        {
            this.img_name = img_name;
            InitializeComponent();
            this.AutoScroll = true;
            Image img = ImageLoader.Instance().getImage(img_name);
            this.pictureBox1.Image = img;
            this.pictureBox1.Size = img.Size;
            this.chip_size = chip_size;
            cursor = new RectCursor(chip_size, chip_size);
        }

        private void ChipSelector_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = this.pictureBox1.PointToClient(mousep);
            cursor.Click(mousep);
            Refresh();

            MainForm main = MainForm.mainform;
            main.setCursorSize(this.chip_size);
            main.ChipSelectorClicked(this);
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {

        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            Image img = ImageLoader.Instance().getImage(img_name);
            if(this.pictureBox1.Image != img)
                this.pictureBox1.Image = img;
            cursor.paint(e.Graphics);
        }

        public Tuple<int, int> getCurrentIndex() 
        {
            return cursor.OriginalIndex();
        }

        public Point getCurrentRefPoint() 
        {
            Tuple<int, int> t = cursor.OriginalIndex();
            return new Point(t.Item1 * chip_size, t.Item2 * chip_size);
        }

        public int ChipSize
        {
            get { return chip_size; }
        }
    }
}
