using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace MapEditor
{
    public class RectCursor
    {
        public RectCursor(int chip_w, int chip_h)
        {
            color = Color.Red;
            ori.X = -1; ori.Y = -1;
            ter.X = -1; ter.Y = -1;
            index_ori_x = index_ori_y = index_ter_x = index_ter_y = 0;
            chip_width = chip_w; chip_height = chip_h;
        }
        public void Click(Point p) 
        {
            ori = ter = p;
            index_ori_x = index_ter_x = p.X / chip_width;
            index_ori_y = index_ter_y = p.Y / chip_height;
        }
        public void Down(Point p) 
        {
            ori = ter = p;
            index_ori_x = index_ter_x = p.X / chip_width;
            index_ori_y = index_ter_y = p.Y / chip_height;
        }
        public void Release(Point p) 
        {
            ter = p;
            index_ter_x = p.X / chip_width;
            index_ter_y = p.Y / chip_height;
        }
        public void paint(Graphics g)
        {
            Pen red2 = new Pen(color, 2);
            g.DrawRectangle(red2, index_ori_x * chip_width, index_ori_y * chip_height
                , (index_ter_x - index_ori_x + 1) * chip_width, (index_ter_y - index_ori_y + 1) * chip_height);
        }
        public RectCursor clone() 
        {
            RectCursor rtn = new RectCursor(this.chip_width, this.chip_height);
            rtn.index_ori_x = this.index_ori_x;
            rtn.index_ori_y = this.index_ori_y;
            rtn.index_ter_x = this.index_ter_x;
            rtn.index_ter_y = this.index_ter_y;
            rtn.ori = this.ori;
            rtn.ter = this.ter;
            return rtn;
        }
        private Point ori, ter;
        private int index_ori_x, index_ori_y, index_ter_x, index_ter_y;
        private int chip_width, chip_height;
        public Color color;

        public Tuple<int, int> OriginalIndex()
        {
            return new Tuple<int, int>(index_ori_x, index_ori_y);
        }
        public Tuple<int, int> TerminalIndex()
        {
            return new Tuple<int, int>(index_ter_x, index_ter_y);
        }
        public void ChangeSize(int w, int h) 
        {
            chip_width = w;
            chip_height = h;
        }
        public Point getOrigin()
        {
            return new Point(index_ori_x * chip_width, index_ori_y * chip_height);
        }
    }
}
