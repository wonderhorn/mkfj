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
    public partial class NewMapDialog : Form
    {
        public NewMapDialog()
        {
            InitializeComponent();
        }

        public String MapName 
        {
            get{return this.textBox1.Text;}
        }
        public int MapWidth
        {
            get { return int.Parse(this.textBox2.Text); }
        }
        public int MapHeight
        {
            get { return int.Parse(this.textBox3.Text); }
        }
    }
}
