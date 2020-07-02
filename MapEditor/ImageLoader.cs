using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;

namespace MapEditor
{
    public class ImageLoader
    {
        static ImageLoader()
        {
            instance = new ImageLoader();
        }
        public ImageLoader()
        {
            img_hash = new Hashtable();
        }

        public Bitmap getImage(String img_name)
        {
            String file_name = Path.GetFileName(img_name);
            if (img_hash.ContainsKey(img_name))
                return (Bitmap)img_hash[img_name];
            else 
            {
                Bitmap bmp = (Bitmap)readImageNonblocking("images\\" + file_name);
                img_hash.Add(img_name, bmp);
                return bmp;
            }
        }

        public void reset()
        {
            img_hash.Clear();
        }

        private static System.Drawing.Image readImageNonblocking(String filename)
        {
            System.IO.FileStream fs = new System.IO.FileStream(
                filename,
                System.IO.FileMode.Open,
                System.IO.FileAccess.Read);
            System.Drawing.Image img = System.Drawing.Image.FromStream(fs);
            fs.Close();
            return img;
        }

        private Hashtable img_hash;

        static ImageLoader instance;
        static public ImageLoader Instance() { return instance; } 
    }
}
