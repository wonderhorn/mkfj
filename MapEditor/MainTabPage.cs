using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{

    public class MapTabPage : TabPage
    {
        private MapData data;
        private List<Billboard> billboards;
        private PictureBox picbox;
        private RectCursor cursor, cursor_yellow;
        private bool isDragging;
        public String saveFileName = "noname";
        public bool changedAfterSave = false;
        public enum SelectingType { Mapchip, Object, Billboard };
        public SelectingType selecting_type;
        //public bool selectingMapchips;
        static public MapTabPage fromFile(String filename)
        {
            String mapname = filename.Split('\\').Last();
            MapTabPage rtn = new MapTabPage(mapname);
            rtn.data = MapData.fromFile(filename, rtn.billboards);
            rtn.picbox.Size = rtn.data.Size;
            rtn.AutoScroll = true;
            rtn.saveFileName = mapname;
            return rtn;
        }
        public MapTabPage(String name)
        {
            this.Name = this.Text = name;
            //data = MapData.MapForTest();
            data = new MapData();
            this.Paint += onPaint;
            picbox = new PictureBox();
            this.Controls.Add(picbox);
            this.AutoScroll = true;
            picbox.Paint += this.onPaint;
            picbox.Click += this.onClick;
            picbox.MouseMove += this.onMouseMove;
            picbox.MouseDown += this.onMouseDown;
            picbox.MouseUp += this.onMouseUp;
            //picbox.PreviewKeyDown += this.onKeyDown;
            //this.PreviewKeyDown += this.onKeyDown;
            cursor = new RectCursor(64, 64);
            cursor_yellow = new RectCursor(64, 64);
            cursor_yellow.color = Color.Yellow;
            isDragging = false;
            //selectingMapchips = true;
            selecting_type = SelectingType.Mapchip;
            billboards = new List<Billboard>();
        }
        public bool save()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.FileName = this.saveFileName;
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                String path = sfd.FileName;
                bool res = data.save(path, billboards);
                sfd.Dispose();
                changedAfterSave = false;
                return res;
            }
            else return false;
        }
        public bool saveImage()
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.FileName = this.saveFileName + ".png";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                String path = sfd.FileName;
                this.picbox.Image = new Bitmap(this.data.Width * 64, this.data.Height * 64);
                this.paint(Graphics.FromImage(this.picbox.Image));
                if (this.picbox.Image != null)
                    this.picbox.Image.Save(path, System.Drawing.Imaging.ImageFormat.Png);
                else
                    MessageBox.Show("Error: 空のマップは画像にエクスポートできません");
                sfd.Dispose();
                //changedAfterSave = false;
                return true;
            }
            else return false;
        }

        void paint(Graphics g)
        {
            foreach (Billboard b in billboards)
            {
                g.DrawImage(ImageLoader.Instance().getImage(b.file_name)
                    , new Rectangle(b.x, b.y, b.w, b.h)
                    , new Rectangle(b.refx, b.refy, b.w, b.h), GraphicsUnit.Pixel);
            }
            this.picbox.Size = data.Size;
            data.paint(g);
            cursor.paint(g);
            cursor_yellow.paint(g);
        }
        void onPaint(object sender, PaintEventArgs e)
        {
            this.paint(e.Graphics);
        }
        void onClick(object sender, EventArgs e)
        {
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = picbox.PointToClient(mousep);
            cursor.Click(mousep);
            Refresh();
        }
        void onMouseMove(object sender, EventArgs e)
        {
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = picbox.PointToClient(mousep);
            if (isDragging)
                cursor_yellow.Release(mousep);
            else
                cursor_yellow.Click(mousep);
            int x = mousep.X / 32 * 32;
            int y = mousep.Y / 32 * 32;
            MainForm.mainform.setMousePoint(new Point(x, y));
            Refresh();
        }
        void onMouseDown(object sender, EventArgs e)
        {
            isDragging = true;
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = picbox.PointToClient(mousep);
            cursor_yellow.Down(mousep);
            Refresh();
        }
        void onMouseUp(object sender, EventArgs e)
        {
            isDragging = false;
            Point mousep = System.Windows.Forms.Cursor.Position;
            mousep = picbox.PointToClient(mousep);
            cursor_yellow.Release(mousep);
            cursor = cursor_yellow.clone();
            cursor.color = Color.Red;
            Refresh();
        }

        private void putChips()
        {
            changedAfterSave = true;
            if (selecting_type == SelectingType.Mapchip)
            {
                Tuple<int, int> t = cursor.OriginalIndex();
                Tuple<int, int> t_ter = cursor.TerminalIndex();
                Point refp = MainForm.mainform.CurrentMapchip();
                if (refp.X != -1)
                {
                    for (int y = t.Item2; y <= t_ter.Item2; y++)
                    {
                        for (int x = t.Item1; x <= t_ter.Item1; x++)
                        {
                            Mapchip c = new Mapchip();
                            c.width = c.height = 64;
                            c.p = new Point(x * 64, y * 64);
                            c.refx = refp.X;
                            c.refy = refp.Y;
                            data[c.p] = c;
                        }
                    }
                }
            }
            else if (selecting_type == SelectingType.Object)
            {
                Tuple<int, int> t = cursor.OriginalIndex();
                Tuple<int, int> t_ter = cursor.TerminalIndex();
                Point refp = MainForm.mainform.CurrentObjchip();
                if (refp.X != -1)
                {
                    for (int y = t.Item2; y <= t_ter.Item2; y++)
                    {
                        for (int x = t.Item1; x <= t_ter.Item1; x++)
                        {
                            Mapchip c = new Mapchip();
                            c.width = c.height = 32;
                            c.p = new Point(x * 32, y * 32);
                            c.refx = refp.X;
                            c.refy = refp.Y;
                            data.setObject(c.p, c);
                        }
                    }
                }
            }
            else if (selecting_type == SelectingType.Billboard)
            {
                Billboard b = MainForm.mainform.bbs.getBillboard();
                Point p = cursor.getOrigin();
                b.x = p.X;
                b.y = p.Y;
                b.translate_rate_y = 1;
                b.translate_rate_x = 0.6;
                billboards.Add(b);
            }
        }
        private void deleteChips()
        {
            changedAfterSave = true;
            //if (selectingMapchips == true)
            if (selecting_type == SelectingType.Mapchip)
            {
                Mapchip c = new Mapchip();
                c.width = c.height = 64;
                Tuple<int, int> t = cursor.OriginalIndex();
                Tuple<int, int> t_ter = cursor.TerminalIndex();

                for (int y = t.Item2; y <= t_ter.Item2; y++)
                {
                    for (int x = t.Item1; x <= t_ter.Item1; x++)
                    {
                        c.p = new Point(x * 64, y * 64);
                        Point prt = new Point(x * 64 + 32, y * 64);
                        Point plb = new Point(x * 64 + 32, y * 64 + 32);
                        Point prb = new Point(x * 64 + 32, y * 64 + 32);
                        c.refx = -1;
                        c.refy = -1;
                        data[c.p] = c;
                        /*data.setObject(c.p, c);
                        data.setObject(prt, c);
                        data.setObject(prb, c);
                        data.setObject(plb, c);*/
                    }
                }
            }
            else if (selecting_type == SelectingType.Object)
            {
                Tuple<int, int> t = cursor.OriginalIndex();
                Tuple<int, int> t_ter = cursor.TerminalIndex();
                Point refp = MainForm.mainform.CurrentObjchip();
                if (refp.X != -1)
                {
                    for (int y = t.Item2; y <= t_ter.Item2; y++)
                    {
                        for (int x = t.Item1; x <= t_ter.Item1; x++)
                        {
                            Mapchip c = new Mapchip();
                            c.width = c.height = 32;
                            c.p = new Point(x * 32, y * 32);
                            c.refx = -1;
                            c.refy = -1;
                            data.setObject(c.p, c);
                        }
                    }
                }
            }
            else if (selecting_type == SelectingType.Billboard) 
            {
                Point p = cursor.getOrigin();
                foreach (Billboard b in billboards)
                {
                    Rectangle rect = new Rectangle(b.x, b.y, b.w, b.h);
                    if (rect.Contains(p))
                    {
                        billboards.Remove(b);
                        break;
                    }
                }
            }
        }
        private void setChipProperty()
        {
            PropertyForm f = new PropertyForm();
            Tuple<int, int> tt = cursor.OriginalIndex();
            //Point pp = selectingMapchips ? new Point(tt.Item1 * 64, tt.Item2 * 64) : new Point(tt.Item1 * 32, tt.Item2 * 32);
            Point pp;// = selectingMapchips ? new Point(tt.Item1 * 64, tt.Item2 * 64) : new Point(tt.Item1 * 32, tt.Item2 * 32);

            if (selecting_type == SelectingType.Mapchip)
            //if (selectingMapchips)
            {
                pp = new Point(tt.Item1 * 64, tt.Item2 * 64);
                if (data[pp] != null)
                {
                    //f.Text = data[pp].etc;
                    if(f.PropertyText != data[pp].etc)
                        changedAfterSave = true;
                    f.PropertyText = data[pp].etc;
                }
            }
            else if (selecting_type == SelectingType.Object)
            {
                pp = new Point(tt.Item1 * 32, tt.Item2 * 32);
                if (data.getObject(pp) != null)
                {
                    //f.Text = data[pp].etc;
                    if (f.PropertyText != data.getObject(pp).etc)
                        changedAfterSave = true;
                    f.PropertyText = data.getObject(pp).etc;
                }
            }
            else if (selecting_type == SelectingType.Billboard) 
            {
            }

            if (f.ShowDialog() == DialogResult.OK)
            {
                if (selecting_type == SelectingType.Mapchip)
                {
                    Mapchip c = new Mapchip();
                    c.width = c.height = 64;
                    Tuple<int, int> t = cursor.OriginalIndex();
                    Tuple<int, int> t_ter = cursor.TerminalIndex();

                    for (int y = t.Item2; y <= t_ter.Item2; y++)
                    {
                        for (int x = t.Item1; x <= t_ter.Item1; x++)
                        {
                            Point p = new Point(x * 64, y * 64);
                            if (data[p] != null)
                            {
                                if (f.PropertyText != data[p].etc)
                                    changedAfterSave = true;
                                data[p].etc = f.PropertyText;
                            }
                        }
                    }
                }
                else if (selecting_type == SelectingType.Object)
                {
                    Tuple<int, int> t = cursor.OriginalIndex();
                    Tuple<int, int> t_ter = cursor.TerminalIndex();
                    Point refp = MainForm.mainform.CurrentObjchip();
                    if (refp.X != -1)
                    {
                        for (int y = t.Item2; y <= t_ter.Item2; y++)
                        {
                            for (int x = t.Item1; x <= t_ter.Item1; x++)
                            {
                                Point p = new Point(x * 32, y * 32);
                                Mapchip chip = data.getObject(p);
                                if (chip != null)
                                {
                                    if (f.PropertyText != chip.etc)
                                        changedAfterSave = true;
                                    chip.etc = f.PropertyText;
                                }
                            }
                        }
                    }
                }
            }
        }
        public void onKeyDown(object sender, KeyEventArgs e)
        {
            if ((e.KeyData & Keys.Enter) == Keys.Enter)
            {
                putChips();
            }
            else if ((e.KeyData & Keys.Delete) == Keys.Delete)
            {
                deleteChips();
            }
            else if ((e.KeyData & Keys.C) == Keys.C && (Control.ModifierKeys & Keys.Control) == Keys.Control)
            {
                MainForm.mainform.Clip(data, cursor);
            }
            else if ((e.KeyData & Keys.E) == Keys.E)
            {
                setChipProperty();
            }

        }

        public RectCursor RCursor
        {
            get { return cursor; }
            set { cursor = value; }
        }
        public MapData Data
        {
            get { return data; }
            set { data = value; }
        }
        public void ChangeCursorSize(int size)
        {
            this.cursor.ChangeSize(size, size);
            this.cursor_yellow.ChangeSize(size, size);
        }
    };

    public class AddTabPage : TabPage
    {
        public AddTabPage()
        {
            this.Name = this.Text = "+";
            //this. += onClick;
        }
        public void onClick(object sender, EventArgs e)
        {
            this.Parent.Controls.Add(new MapTabPage("noname"));
        }
    };
}
