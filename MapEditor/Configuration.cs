using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;
using System.IO;

namespace MapEditor
{
    public class Configuration
    {
        public static Configuration instance;
        const String filename = "config.txt";
        private Hashtable table;
        public Configuration()
        {
            table = new Hashtable();
        }
        ~Configuration()
        {
            Save();
        }
        public String this[String key]
        {
            get { return ((String)table[key]); }
            set { table[key] = value; }
        }
        public bool Save() 
        {
            StreamWriter sw = new StreamWriter(filename);
            foreach (DictionaryEntry de in table) 
            {
                String line = ((String)de.Key) + " " + ((String)de.Value);
                sw.WriteLine(line);
            }
            sw.Close();
            return true;
        }
        public bool Load() 
        {
            try
            {
                StreamReader sr = new StreamReader(filename);
                while (!sr.EndOfStream)
                {
                    String line = sr.ReadLine();
                    int p = line.IndexOf(" ");
                    if (p >= 0) 
                    {
                        String key = line.Substring(0, p);
                        String value = line.Substring(p + 1);
                        table[key] = value;
                    }
                }
                sr.Close();
            }
            catch (System.IO.FileNotFoundException) 
            {
                return false;
            }
            
            return true;
        }
    }
}
