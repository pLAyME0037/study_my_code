using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Xml;

namespace RegistrationApp
{
    class connection
    {
        private XmlTextReader reader = new XmlTextReader("connection.xml");
        public string connString;

        public connection()
        {
            string address = null;
            string db_name = null;

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
            connString = @"Data Source="
            + address
            + ",1433;Network Library=DBMSSOCN;Initial Catalog="
            + db_name
            + ";User ID=register;Password=12345678;";
        }
        public string name
        {
            set { connString = value; }
        }
    }
}
