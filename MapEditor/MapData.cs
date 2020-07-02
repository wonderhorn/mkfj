using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;

namespace MapEditor
{
    public class Mapchip 
    {
        public Point p;
        public int width, height;
        public int refx, refy;
        public String etc;
        public Mapchip()
        {
            p.X = p.Y = refx = refy = -1;
        }
    };
    public class MapData
    {
        private Dictionary<Point, Mapchip> chips, objects;
        private int width, height, chip_width, chip_height, obj_width, obj_height;
        private String name;
        private int bg_offset_x = 0, bg_offset_y = 0
            , bg_image_width = 640, bg_image_height = 480
            , bg_x_max = 999999, bg_x_min = -999999
            , bg_y_max = 999999, bg_y_min = -999999;
        private double bg_translate_rate_x = 1.0;
        private double bg_translate_rate_y = 1.0;
        private String background = "school";
        private String bgm_name = "none";
        private double bgm_segno = 0;
        public MapData()
        {
            width = height = 0;
            chip_width = chip_height = 64;
            obj_width = obj_height = 32;
            chips = new Dictionary<Point, Mapchip>();
            objects = new Dictionary<Point, Mapchip>();
            name = "noname";
        }

        public int Width 
        {
            get { return width; }
            set { width = value; }
        }
        public int Height
        {
            get { return height; }
            set { height = value; }
        }

        public Mapchip this[Point p]
        {
            get 
            {
                if (chips.ContainsKey(p))
                    return chips[p];
                else return null;
            }
            set
            {
                if (chips.ContainsKey(p))
                    chips[p] = value;
                else chips.Add(p, value) ;
            }
        }
        public Mapchip getObject(Point p)
        {
            if (objects.ContainsKey(p))
                return objects[p];
            else return null;
        }
        public void setObject(Point p, Mapchip c)
        {

            if (objects.ContainsKey(p))
                objects[p] = c;
            else objects.Add(p, c);
        }

        public void paint(Graphics g)
        {
            foreach (KeyValuePair<Point, Mapchip> kv in chips)
            {
                if (kv.Value.refx >= 0)
                    g.DrawImage(ImageLoader.Instance().getImage(Constants.MapchipImageFileName)
                        , new Rectangle(kv.Key, new Size(kv.Value.width, kv.Value.height))
                        , new Rectangle(kv.Value.refx, kv.Value.refy, kv.Value.width, kv.Value.height), GraphicsUnit.Pixel);
            }
            foreach (KeyValuePair<Point, Mapchip> kv in objects)
            {
                if (kv.Value.refx >= 0)
                    g.DrawImage(ImageLoader.Instance().getImage(Constants.ObjectImageFileName)
                        , new Rectangle(kv.Key, new Size(kv.Value.width, kv.Value.height))
                        , new Rectangle(kv.Value.refx, kv.Value.refy, kv.Value.width, kv.Value.height), GraphicsUnit.Pixel);
            }
        }

        static public MapData MapForTest() 
        {
            MapData map = new MapData();
            map.name = "test";
            map.width = 10;
            map.height = 8;
            for (int y = 0; y < map.height; y++ )
            {
                for (int x = 0; x < map.width; x++) 
                {
                    Mapchip chip = new Mapchip();
                    chip.width = chip.height = 64;
                    chip.refx = chip.refy = 0;
                    Point p = new Point(x * chip.width, y * chip.height);
                    chip.p = p;
                    map[p] = chip;
                }
            }
            return map;
        }
        public Size Size 
        {
            get { return new Size(width * chip_width, height * chip_height); }
        }
        static public MapData fromFile(String filename, List<Billboard> billboards) 
        {
            MapData rtn = new MapData();
            StreamReader sr = new StreamReader(filename, System.Text.Encoding.GetEncoding("shift_jis"));
            billboards.Clear();
            while (!sr.EndOfStream) 
            {
                String line = sr.ReadLine();
                String[] tokens = line.Split(' ');
                String verb = tokens[0];
                if(verb.Equals("name") )
                {
                    rtn.name = tokens[1];
                }
                else if (verb.Equals("width")) 
                {
                    rtn.width = int.Parse(tokens[1]);
                }
                else if (verb.Equals("height")) 
                {
                    rtn.height = int.Parse(tokens[1]);
                }
                else if (verb.Equals("mapchip")) 
                {
                    Mapchip c = new Mapchip();
                    if (tokens.Length <= 6)
                        continue;
                    c.p = new Point(int.Parse(tokens[1]), int.Parse(tokens[2]));
                    c.refx = int.Parse(tokens[3]);
                    c.refy = int.Parse(tokens[4]);
                    c.width = int.Parse(tokens[5]);
                    c.height = int.Parse(tokens[6]);
                    if(c.refx >= 0)
                        rtn.chips.Add(c.p, c);
                    if (tokens.Length >= 8)
                        c.etc = tokens[7];

                }
                else if (verb.Equals("object"))
                {
                    Mapchip c = new Mapchip();
                    if (tokens.Length <= 6)
                        continue;
                    c.p = new Point(int.Parse(tokens[1]), int.Parse(tokens[2]));
                    c.refx = int.Parse(tokens[3]);
                    c.refy = int.Parse(tokens[4]);
                    c.width = int.Parse(tokens[5]);
                    c.height = int.Parse(tokens[6]);
                    if (tokens.Length == 8)
                        c.etc = tokens[7];
                    if (c.refx >= 0)
                        rtn.objects.Add(c.p, c);
                }
                else if (verb.Equals("billboard"))
                {
                    Billboard c = new Billboard();
                    if (tokens.Length <= 7)
                        continue;
                    c.x = int.Parse(tokens[1]);
                    c.y = int.Parse(tokens[2]);
                    c.refx = int.Parse(tokens[3]);
                    c.refy = int.Parse(tokens[4]);
                    c.w = int.Parse(tokens[5]);
                    c.h = int.Parse(tokens[6]);
                    c.translate_rate_x = double.Parse(tokens[7]);
                    c.translate_rate_y = double.Parse(tokens[8]);
                    c.file_name = (tokens[9]);
                    c.file_name = Path.GetFileName(c.file_name);

                    for (int i = 10; i < tokens.Length; i++)
                    {
                        c.file_name += (" " + tokens[i]);
                    }
                    if (c.refx >= 0)
                        billboards.Add(c);
                }
                else if (verb.Equals("background"))
                {
                    rtn.background = tokens[1];
                }
                else if (verb.Equals("bg_offset_x")) 
                {
                    rtn.bg_offset_x = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_offset_y"))
                {
                    rtn.bg_offset_y = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_x_min"))
                {
                    rtn.bg_x_min = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_x_max"))
                {
                    rtn.bg_x_max = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_y_min"))
                {
                    rtn.bg_y_min = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_y_max"))
                {
                    rtn.bg_y_max = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_image_width"))
                {
                    rtn.bg_image_width = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_image_height"))
                {
                    rtn.bg_image_height = int.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_translate_rate"))
                {
                    rtn.bg_translate_rate_x = rtn.bg_translate_rate_y
                        = double.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_translate_rate"))
                {
                    rtn.bg_translate_rate_x
                        = rtn.bg_translate_rate_y
                        = double.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_translate_rate_x"))
                {
                    rtn.bg_translate_rate_x = double.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_translate_rate_y"))
                {
                    rtn.bg_translate_rate_y = double.Parse(tokens[1]);
                }
                else if (verb.Equals("bg_translate_rate_y"))
                {
                    rtn.bg_translate_rate_y = double.Parse(tokens[1]);
                }
                else if (verb.Equals("bgm"))
                {
                    rtn.bgm_name = tokens[1];
                    rtn.bgm_segno = double.Parse(tokens[2]);
                }
            }
            sr.Close();
            return rtn;
        }
        public bool save(String filename, List<Billboard> billboards)
        {
            StreamWriter sw = new StreamWriter(filename, false);
            sw.WriteLine("name " + this.name);
            sw.WriteLine("width " + this.width);
            sw.WriteLine("height " + this.height);
            sw.WriteLine("background " + this.background);
            sw.WriteLine("bg_image_width " + this.bg_image_width);
            sw.WriteLine("bg_image_height " + this.bg_image_height);
            sw.WriteLine("bg_offset_x " + this.bg_offset_x);
            sw.WriteLine("bg_offset_y " + this.bg_offset_y);
            sw.WriteLine("bg_x_min " + this.bg_x_min);
            sw.WriteLine("bg_x_max " + this.bg_x_max);
            sw.WriteLine("bg_y_min " + this.bg_y_min);
            sw.WriteLine("bg_y_max " + this.bg_y_max);
            sw.WriteLine("bg_translate_rate_x " + this.bg_translate_rate_x);
            sw.WriteLine("bg_translate_rate_y " + this.bg_translate_rate_y);
            sw.WriteLine("bgm " + this.bgm_name + " " + this.bgm_segno);
            foreach(KeyValuePair<Point, Mapchip> kv in chips)
            {
                Mapchip c = kv.Value;
                if (c.refx >= 0)
                {
                    sw.WriteLine("mapchip " + c.p.X.ToString() + " " + c.p.Y.ToString() + " "
                         + c.refx.ToString() + " " + c.refy.ToString() + " "
                          + c.width.ToString() + " " + c.height.ToString() + " " + c.etc);
                }
            }
            foreach (KeyValuePair<Point, Mapchip> kv in objects)
            {
                Mapchip c = kv.Value;
                if (c.refx >= 0)
                {
                    sw.WriteLine("object " + c.p.X.ToString() + " " + c.p.Y.ToString() + " "
                         + c.refx.ToString() + " " + c.refy.ToString() + " "
                          + c.width.ToString() + " " + c.height.ToString() + " " + c.etc);
                }
            }

            foreach(Billboard b in billboards)
            {
                String[] tokens = b.file_name.Split('\\');
                String file_name;
                if(tokens.Length > 2)
                    file_name = tokens[tokens.Length - 2] + "\\" + tokens[tokens.Length - 1];
                else
                    file_name = "images" + "\\" + tokens[tokens.Length - 1];
                sw.WriteLine("billboard " + b.x.ToString() + " " + b.y.ToString() + " "
                     + b.refx.ToString() + " " + b.refy.ToString() + " "
                      + b.w.ToString() + " " + b.h.ToString() + " "
                      + b.translate_rate_x + " " +b.translate_rate_y + " " + file_name);
            }

            sw.Close();
            return true;
        }
    }
}
