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
    class UserModel
    {
        // Attributes of Class(User)
        public int UserId { get; set; }
        public int UserTypeId { get; set; }
        public string UserTypeName { get; set; }
        public string Username { get; set; }
        public string Fullname { get; set; }
        public string Sex { get; set; }
        public string Password { get; set; }
        public DateTime C_Date { get; set; }
        public int C_UserId { get; set; }
        public Boolean EnableStatus { get; set; }
        //Public Class
        connection newconnection = new connection();
        //Functions
        public string UserLogin(string Username, string Password)
        {
            string Result = "Warning! Invalid data input.";
            try
            {
                SqlConnection cnn = new SqlConnection();
                SqlCommand cmd = new SqlCommand();

                //connectionString with Connection Class file
                cnn.ConnectionString = newconnection.connString;

                //ConnectionString with App.Config
                //cnn.ConnectionString = ConfigurationManager.ConnectionStrings["ConnectionStringName"].ConnectionString;

                //Direct connectionString
                //cnn.ConnectionString = "Data Source=127.0.0.1,1433;Initial Catalog = RegistrationApp; Integrated Security = False; User ID = vbt; Password = 1122334455; MultipleActiveResultSets = True";

                cnn.Open();
                cmd.Connection = cnn;
                cmd.Parameters.Clear();
                cmd.Parameters.AddWithValue("@Username", Username);
                cmd.Parameters.AddWithValue("@Password", Password);
                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "UserLogin";
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
        public int UserIndentification(String Username)
        {
            int Result = 0;

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();
            cmd.Parameters.AddWithValue("@Username", Username);
            //Create out Parameter
            IDbDataParameter ParmError = cmd.CreateParameter();
            ParmError.ParameterName = "@UserId";
            ParmError.Direction = System.Data.ParameterDirection.Output;
            ParmError.DbType = System.Data.DbType.Int32;
            //ParmError.Size = 100;
            cmd.Parameters.Add(ParmError);
            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "UserIndentification";
            cmd.ExecuteNonQuery();
            int Error = Convert.ToInt32(ParmError.Value);
            
            Result = Error;
            cnn.Close();

            return Result;
        }
        public UserModel UserInfor(int UserId)
        {
            UserModel MD = new UserModel();

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.Parameters.AddWithValue("@UserId", UserId);

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "UserInfor";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                MD.UserId = Convert.ToInt32(dr["UserId"]);
                MD.UserTypeId = Convert.ToInt32(dr["UserTypeId"]);
                MD.UserTypeName = dr["UserTypeName"].ToString();
                MD.Username = dr["Username"].ToString();
                MD.Fullname = dr["Fullname"].ToString();

                MD.Sex = dr["Sex"].ToString();
                MD.C_Date = Convert.ToDateTime(dr["C_Date"]);
                MD.EnableStatus = Convert.ToBoolean(dr["EnableStatus"]);
            }
            cnn.Close();

            return MD;
        }
        public List<UserModel> UserList ()
        {
            List<UserModel> TableList = new List<UserModel>();

            SqlConnection cnn = new SqlConnection();
            SqlCommand cmd = new SqlCommand();
            SqlDataReader dr;
            cnn.ConnectionString = newconnection.connString;

            cnn.Open();
            cmd.Connection = cnn;
            cmd.Parameters.Clear();

            cmd.CommandType = CommandType.StoredProcedure;
            cmd.CommandText = "UserList";

            dr = cmd.ExecuteReader();
            while (dr.Read())
            {
                TableList.Add(new UserModel()
                {
                    UserId = Convert.ToInt32(dr[0]),
                    UserTypeId = Convert.ToInt32(dr[1]),
                    UserTypeName = dr[2].ToString(),
                    Username = dr[3].ToString(),
                    Fullname = dr[4].ToString(),

                    Sex = dr[5].ToString(),
                    C_Date = Convert.ToDateTime(dr[6]),
                    EnableStatus = Convert.ToBoolean(dr[7])
                });
            }
            cnn.Close();


            return TableList;
        }
        public string UserAdd(int UserTypeId,string Username,string Fullname,string Sex, string Password, string @ConfirmedPassword, int UserId)
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
                cmd.Parameters.AddWithValue("@UserTypeId", UserTypeId);
                cmd.Parameters.AddWithValue("@Username", Username);
                cmd.Parameters.AddWithValue("@Fullname", Fullname);
                cmd.Parameters.AddWithValue("@Sex", Sex);
                cmd.Parameters.AddWithValue("@Password", Password);
                
                cmd.Parameters.AddWithValue("@ConfirmedPassword", ConfirmedPassword);
                cmd.Parameters.AddWithValue("@UserId", UserId);

                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "UserAdd";
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
        public string UserDelete(int Id, int UserId)
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
                cmd.CommandText = "UserDelete";
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
        public string UserChange(int Id, int TypeId, string Fullname, string Sex,int UserId)
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
                cmd.Parameters.AddWithValue("@TypeId", TypeId);
                cmd.Parameters.AddWithValue("@Fullname", Fullname);
                cmd.Parameters.AddWithValue("@Sex", Sex);
                cmd.Parameters.AddWithValue("@UserId", UserId);              

                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "UserInfoChange";
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
        public string UserChangeStatus(int Id, int UserId)
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
                cmd.CommandText = "UserChangeStatus";
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

        public string UserChangePassword(int Id, string NewPassword, string NewConfirmPassword, string Password,int UserId)
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
                cmd.Parameters.AddWithValue("@NewPassword", NewPassword);
                cmd.Parameters.AddWithValue("@NewConfirmPassword", NewConfirmPassword);                
                
                cmd.Parameters.AddWithValue("@Password", Password);
                cmd.Parameters.AddWithValue("@UserId", UserId);

                //Create out Parameter
                IDbDataParameter ParmError = cmd.CreateParameter();
                ParmError.ParameterName = "@Err";
                ParmError.Direction = System.Data.ParameterDirection.Output;
                ParmError.DbType = System.Data.DbType.String;
                ParmError.Size = 100;
                cmd.Parameters.Add(ParmError);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.CommandText = "UserChangePassword";
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
    }
}
