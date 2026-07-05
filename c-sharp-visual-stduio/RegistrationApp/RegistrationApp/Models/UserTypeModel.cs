using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Configuration;
using System.Data.SqlClient;
using System.Data;

using System.Xml;


namespace RegistrationApp.Models
{
    class UserTypeModel
    {
        public int UserTypeId { get; set; }
        public string UserTypeName { get; set; }

        connection newconnection = new connection();

        public List<UserTypeModel> UserTypeList()
        {
            List<UserTypeModel> TableList = new List<UserTypeModel>();

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            //cnn.ConnectionString = ConfigurationManager.ConnectionStrings["ConnectionStringName"].ConnectionString;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "UserTypeList";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                TableList.Add(new UserTypeModel()
                {
                    UserTypeId = Convert.ToInt32(dr[0]),
                    UserTypeName = dr[1].ToString(),                   
                });
            }
            cnn.Close();
            return TableList;

        }
    }
}
