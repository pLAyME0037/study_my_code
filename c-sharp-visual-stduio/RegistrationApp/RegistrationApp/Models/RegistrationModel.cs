using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data.SqlClient;
using System.Data;

namespace RegistrationApp.Models
{
    class RegistrationModel
    {
        public int RegistrationId { get; set; }
        public string StudentCode { get; set; }
        public string Name { get; set; }
        public string Sex { get; set; }
        public DateTime DoB { get; set; }
        public DateTime C_Date { get; set; }
        public int C_UserId { get; set; }
        //Public Class
        connection newconnection = new connection();
        //Function
        public string RegistrationAdd(string StudentCode, string Name, string Sex, DateTime DoB,int UserId)
        {
            string Result = "Invalid Data input, please try again.";
            try
            {
                SqlConnection cnn = new SqlConnection();
                SqlCommand cmd = new SqlCommand();
                cnn.ConnectionString = newconnection.connString;

                cnn.Open();
                cmd.Connection = cnn;
                cmd.Parameters.Clear();
                cmd.Parameters.AddWithValue("@StudentCode", StudentCode);
                cmd.Parameters.AddWithValue("@Name", Name);
                cmd.Parameters.AddWithValue("@Sex", Sex);
                cmd.Parameters.AddWithValue("@DoB", DoB);

                cmd.Parameters.AddWithValue("@UserId", UserId);

                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "RegistrationAdd";
                cmd.ExecuteNonQuery();
                string Error = ParmError.Value.ToString();
                Result = Error;
                cnn.Close();
            }
            catch (Exception)
            {
                Result = "DB Error. More infor, pls contact the admin user.";
            }
            return Result;
        }
        public List<RegistrationModel> RegistrationList()
        {
            List<RegistrationModel> TableList = new List<RegistrationModel>();

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "RegistrationList";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                TableList.Add(new RegistrationModel()
                {
                    RegistrationId = Convert.ToInt32(dr[0]),
                    StudentCode = dr[1].ToString(),
                    Name = dr[2].ToString(),
                    Sex = dr[3].ToString(),
                    DoB = Convert.ToDateTime(dr[4]),

                    C_Date = Convert.ToDateTime(dr[5]),
                    C_UserId = Convert.ToInt32(dr[6])
                });
            }
            cnn.Close();
            return TableList;
        }
        public DataTable RegistrationList_DTT()
        {
            DataTable Dtt = new DataTable();
            
            DataRow R;
            Dtt.Columns.Add("RegistrationId", Type.GetType("System.String"));
            Dtt.Columns.Add("StudentCode", Type.GetType("System.String"));
            Dtt.Columns.Add("Name", Type.GetType("System.String"));
            Dtt.Columns.Add("Sex", Type.GetType("System.String"));
            Dtt.Columns.Add("DoB", Type.GetType("System.String"));

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "RegistrationList";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                R = Dtt.NewRow();
                R["RegistrationId"] = dr[0].ToString();
                R["StudentCode"] = dr[1].ToString();
                R["Name"] = dr[2].ToString();
                R["Sex"] = dr[3].ToString();
                R["DoB"] = Convert.ToDateTime(dr[4]).ToString("dd/MMM/yyyy");
                Dtt.Rows.Add(R);
            }
            cnn.Close();

            

            return Dtt;
        }
        public string RegistrationDelete(int Id, int UserId)
        {
            string Result = "Invalid Data input, please try again.";
            try
            {
                SqlConnection cnn = new SqlConnection();
                SqlCommand cmd = new SqlCommand();
                cnn.ConnectionString = newconnection.connString;

                cnn.Open();
                cmd.Connection = cnn;
                cmd.Parameters.Clear();
                cmd.Parameters.AddWithValue("@Id", Id);
                cmd.Parameters.AddWithValue("@UserId", UserId);

                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "RegistrationDelete";
                cmd.ExecuteNonQuery();
                string Error = ParmError.Value.ToString();
                Result = Error;
                cnn.Close();
            }
            catch (Exception)
            {
                Result = "DB Error. More infor, pls contact the admin user.";
            }
            return Result;
        }
        public RegistrationModel RegistrationInfo(int Id)
        {
            RegistrationModel MD = new RegistrationModel();

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.Parameters.AddWithValue("@Id", Id);

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "RegistrationInfo";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                MD.RegistrationId = Convert.ToInt32(dr["RegistrationId"]);
                MD.StudentCode = dr["StudentCode"].ToString();
                MD.Name = dr["Name"].ToString();
                MD.Sex = dr["Sex"].ToString();
                MD.DoB = Convert.ToDateTime(dr["DoB"]);

                MD.C_Date = Convert.ToDateTime(dr["C_Date"]);
                MD.C_UserId = Convert.ToInt32(dr["C_UserId"]);
            }
            cnn.Close();

            return MD;
        }

    }
}
