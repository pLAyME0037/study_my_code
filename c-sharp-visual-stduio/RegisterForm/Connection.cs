Models\Connection.cs
using System;
using System.Xml;

namespace DesktopApp.Models
{
    public class Connection
    {
        public string ConnString { get; private set; }

        public Connection()
        {
            string address = null;
            string db_name = null;
            try
            {
                var path = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "connection.xml");
                if (!System.IO.File.Exists(path))
                {
                    ConnString = string.Empty;
                    return;
                }

                using (var reader = new XmlTextReader(path))
                {
                    while (reader.Read())
                    {
                        if (reader.NodeType == XmlNodeType.Element && reader.Name == "serveraddress")
                        {
                            address = reader.ReadString();
                        }
                        if (reader.NodeType == XmlNodeType.Element && reader.Name == "db_name")
                        {
                            db_name = reader.ReadString();
                        }
                    }
                }

                ConnString = @"Data Source=" + address + ",1433;Network Library=DBMSSOCN;Initial Catalog=" + db_name + ";User ID=vbt;Password=1122334455;";
            }
            catch
            {
                ConnString = string.Empty;
            }
        }
    }
}