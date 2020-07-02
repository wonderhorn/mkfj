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
    public partial class BillBoardSelector : Form
    {
        private String img_file_name;
        private Image img;
        private Point cursor_tmp = new Point(-1, -1), cursor_set = new Point(-1, -1);
        private int cursor_w = 0, cursor_h = 0;
        private const int cursor_size = 32;
        private bool is_mouse_down = false;
        public BillBoardSelector()
        {
            InitializeComponent();
            this.panel1.Anchor = AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom | AnchorStyles.Left;
            this.pictureBox1.Parent = this.panel1;
            this.panel1.AutoScroll = true;
        }

        public static BillBoardSelector create(String img_name) 
        {
            BillBoardSelector bbs = new BillBoardSelector();
            bbs.img_file_name = img_name;
            bbs.img = Bitmap.FromFile(img_name);
            bbs.pictureBox1.Size = bbs.img.Size;
            return bbs;
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = this.pictureBox1.PointToClient(mousep);
            int mx = mousep.X / cursor_size * cursor_size;
            int my = mousep.Y / cursor_size * cursor_size;            
            if (is_mouse_down)
            {
                cursor_w = mx - cursor_set.X + cursor_size;
                cursor_h = my - cursor_set.Y + cursor_size;
            }
            else 
            {
                cursor_tmp = new Point(mx, my);
            }
            this. Refresh();
            changeSelectingType();
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            is_mouse_down = true;
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = this.pictureBox1.PointToClient(mousep);
            int mx = mousep.X / cursor_size * cursor_size;
            int my = mousep.Y / cursor_size * cursor_size;            
            
            cursor_set = new Point(mx, my);
            changeSelectingType();
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            is_mouse_down = false;
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.DrawImage(img, new Point(0, 0));

            g.DrawRectangle(Pens.Yellow, new Rectangle(cursor_set, new Size(cursor_w, cursor_h)));
            g.DrawRectangle(Pens.Red, new Rectangle(cursor_tmp, new Size(cursor_size, cursor_size)));
        }

        private void BillBoardSelector_Click(object sender, EventArgs e)
        {
            MainForm main = MainForm.mainform;
            MapTabPage mtp = main.getSelectedTab();
            mtp.selecting_type = MapTabPage.SelectingType.Billboard;
            main.BillboardSelectorClicked();
        }

        private void changeSelectingType() 
        {
            MainForm main = MainForm.mainform;
            MapTabPage mtp = main.getSelectedTab();
            mtp.selecting_type = MapTabPage.SelectingType.Billboard;
            main.setCursorSize(cursor_size);
            main.BillboardSelectorClicked();
        
        }

        public Billboard getBillboard() 
        {
            Billboard rtn = new Billboard();
            rtn.x = -1;
            rtn.y = -1;
            rtn.refx = cursor_set.X;
            rtn.refy = cursor_set.Y;
            rtn.w = cursor_w;
            rtn.h = cursor_h;
            rtn.file_name = img_file_name;
            return rtn;
        }
    }
}
