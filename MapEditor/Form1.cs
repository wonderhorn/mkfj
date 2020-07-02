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
    public partial class MainForm : Form
    {
        private ChipSelector mapchip_selector, object_selector;
        private ClipBoard clipboard;
        private int cursor_size;
        static public MainForm mainform;
        public BillBoardSelector bbs;
        public MainForm()
        {
            cursor_size = 64;
            mainform = this;
            InitializeComponent();
            mapchip_selector = new ChipSelector(Constants.MapchipImageFileName);
            //mapchip_selector.Parent = this;
            mapchip_selector.Show();
            object_selector = new ChipSelector(Constants.ObjectImageFileName, 32);
            //object_selector.Parent = this;
            object_selector.Show();

            //this.PreviewKeyDown += this.onKeyDown;
            this.KeyDown += this.onKeyDown;
            this.tabControl1.KeyDown += this.onKeyDown;
            this.tabControl1.Controls.Add(new MapTabPage("noname"));
            this.tabControl1.Anchor = AnchorStyles.Bottom 
                | AnchorStyles.Right | AnchorStyles.Top | AnchorStyles.Left;
            this.tabControl1.DrawMode = TabDrawMode.OwnerDrawFixed;
            this.tabControl1.DrawItem += this.DrawCloseTab;
            //this.tabControl1.Controls.Add(new AddTabPage());
            clipboard = new ClipBoard();
            clipboard.Show();
        }

        public void setMousePoint(Point p) 
        {
            this.toolStripStatusLabel1.Text = p.X.ToString() + ", " + p.Y.ToString();
        }

        void onKeyDown(object sender, KeyEventArgs e)
        {
            int a = 0;
            MapTabPage mtp = (MapTabPage)this.tabControl1.SelectedTab;
            mtp.onKeyDown(sender, e);

        }

        private void マップを保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MapTabPage mtp = (MapTabPage)this.tabControl1.SelectedTab;
            if(mtp != null)
                mtp.save();

        }

        private void マップを開くToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                this.tabControl1.Controls.Add(MapTabPage.fromFile(ofd.FileName));
                ofd.Dispose();
            }
        }

        public void Clip(MapData data, RectCursor cursor)
        {
            clipboard.setChips(data, cursor.OriginalIndex().Item1, cursor.OriginalIndex().Item2
                , cursor.TerminalIndex().Item1 - cursor.OriginalIndex().Item1 + 1
                , cursor.TerminalIndex().Item2 - cursor.OriginalIndex().Item2 + 1);
        }

        private void 新規作成ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewMapDialog nmd = new NewMapDialog();
            if (nmd.ShowDialog() == DialogResult.OK)
            {
                MapTabPage mtp = new MapTabPage(nmd.MapName);
                mtp.Data.Width = nmd.MapWidth;
                mtp.Data.Height = nmd.MapHeight;
                this.tabControl1.Controls.Add(mtp);
                nmd.Dispose();
            }
        }

        public void setCursorSize(int size)
        {
            this.cursor_size = size;
            MapTabPage mtp = (MapTabPage)this.tabControl1.SelectedTab;
            mtp.ChangeCursorSize(size);
        }

        public Point CurrentMapchip()
        {
            return mapchip_selector.getCurrentRefPoint();
        }
        public Point CurrentObjchip()
        {
            return object_selector.getCurrentRefPoint();
        }
        public MapTabPage getSelectedTab() 
        {
            return (MapTabPage)this.tabControl1.SelectedTab;
        }
        public void ChipSelectorClicked(ChipSelector cs)
        {
            MapTabPage mtp = (MapTabPage)this.tabControl1.SelectedTab;
            if (cs == this.mapchip_selector)
            {
                mtp.selecting_type = MapTabPage.SelectingType.Mapchip;
            }
            else
            {
                mtp.selecting_type = MapTabPage.SelectingType.Object;
            }
        }
        public void BillboardSelectorClicked() 
        {
            MapTabPage mtp = (MapTabPage)this.tabControl1.SelectedTab;
            mtp.selecting_type = MapTabPage.SelectingType.Billboard;
        }

        private void ビルボード選択ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK) 
            {
                String file_name = ofd.FileName;
                bbs = BillBoardSelector.create(file_name);
                bbs.Show(this);
            }
        }

        private void tabControl1_MouseClick(object sender, MouseEventArgs e)
        {
            Point p = e.Location;
            Rectangle r = this.tabControl1.GetTabRect(tabControl1.SelectedIndex);
            r.X = r.X + r.Width - 8;
            r.Width = 8;
            if (r.Contains(p)) {
                MapTabPage tab = 
                    (MapTabPage)this.tabControl1.TabPages[tabControl1.SelectedIndex];
                if (tab.changedAfterSave)
                {
                    DialogResult result = MessageBox.Show(
                        tab.saveFileName + "は保存されていません\n変更を破棄してよろしいですか？",
                        "変更の破棄",
                        MessageBoxButtons.YesNo,
                        MessageBoxIcon.Exclamation,
                        MessageBoxDefaultButton.Button2
                        );
                    if (result != DialogResult.Yes)
                        return;
                }
                this.tabControl1.TabPages.RemoveAt(tabControl1.SelectedIndex);
            }
        }

        private void 画像として保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MapTabPage tab =
                    (MapTabPage)this.tabControl1.SelectedTab;
            tab.saveImage();
        }

        private void 画像データ再読み込みToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImageLoader.Instance().reset();
        }

        public void DrawCloseTab(object sender,
           System.Windows.Forms.DrawItemEventArgs e)
        {
            Rectangle r = this.tabControl1.GetTabRect(e.Index);
            Font f = this.Font;
            Brush bb = new SolidBrush(Color.Black);
            Brush bg = new SolidBrush(Color.Gray);
            e.Graphics.DrawString(this.tabControl1.TabPages[e.Index].Text, 
                f, bb, r.X, r.Y);
            e.Graphics.DrawString(" x ", f, bg, r.X + r.Width - 12, r.Y);
        }
    }
}
