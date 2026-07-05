using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;

namespace DesktopApp.Models
{
    public class UserModel
    {
        // Public properties (kept for compatibility)
        public int UserId { get; set; }
        public int UserTypeId { get; set; }
        public string UserTypeName { get; set; }
        public string Username { get; set; }
        public string Fullname { get; set; }
        public string Sex { get; set; }
        public string Password { get; set; }
        public DateTime C_Date { get; set; }
        public int C_UserId { get; set; }
        public bool EnableStatus { get; set; }

        private Connection _newConnection;

        // Add this constructor to accept a connection string
        public UserModel(string connectionString)
        {
            // If you have a Connection class, initialize it here
            _newConnection = new Connection(connectionString);
        }

        public string UserLogin(string username, string password)
        {
            var result = "Warning! Invalid data input.";
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserLogin", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@Username", username);
                    cmd.Parameters.AddWithValue("@Password", password);

                    var parm = cmd.CreateParameter();
                    parm.ParameterName = "@Err";
                    parm.Direction = ParameterDirection.Output;
                    parm.DbType = DbType.String;
                    parm.Size = 100;
                    cmd.Parameters.Add(parm);

                    cnn.Open();
                    cmd.ExecuteNonQuery();

                    result = parm.Value?.ToString() ?? result;
                }
            }
            catch (Exception)
            {
                result = "DB Error. More info, please contact the admin user.";
            }
            return result;
        }

        public int UserIndentification(string username)
        {
            var result = 0;
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserIndentification", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@Username", username);

                    var parm = cmd.CreateParameter();
                    parm.ParameterName = "@UserId";
                    parm.Direction = ParameterDirection.Output;
                    parm.DbType = DbType.Int32;
                    cmd.Parameters.Add(parm);

                    cnn.Open();
                    cmd.ExecuteNonQuery();

                    result = Convert.ToInt32(parm.Value);
                }
            }
            catch (Exception)
            {
                result = 0;
            }
            return result;
        }

        public UserModel UserInfor(int userId)
        {
            // Pass the required connectionString argument to the UserModel constructor
            var md = new UserModel(_newConnection.ConnString);
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserInfor", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@UserId", userId);
                    cnn.Open();
                    using (var dr = cmd.ExecuteReader())
                    {
                        while (dr.Read())
                        {
                            md.UserId = Convert.ToInt32(dr["UserId"]);
                            md.UserTypeId = Convert.ToInt32(dr["UserTypeId"]);
                            md.UserTypeName = dr["UserTypeName"].ToString();
                            md.Username = dr["Username"].ToString();
                            md.Fullname = dr["Fullname"].ToString();
                            md.Sex = dr["Sex"].ToString();
                            md.C_Date = Convert.ToDateTime(dr["C_Date"]);
                            md.EnableStatus = Convert.ToBoolean(dr["EnableStatus"]);
                        }
                    }
                }
            }
            catch (Exception)
            {
                // swallow - return default object
            }
            return md;
        }

        public List<UserModel> UserList()
        {
            var list = new List<UserModel>();
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserList", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cnn.Open();
                    using (var dr = cmd.ExecuteReader())
                    {
                        while (dr.Read())
                        {
                            list.Add(new UserModel(_newConnection.ConnString)
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
                    }
                }
            }
            catch (Exception)
            {
            }
            return list;
        }

        public string UserAdd(int userTypeId, string username, string fullname, string sex, string password, string confirmedPassword, int userId)
        {
            var result = "Invalid Data input, please try again.";
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserAdd", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@UserTypeId", userTypeId);
                    cmd.Parameters.AddWithValue("@Username", username);
                    cmd.Parameters.AddWithValue("@Fullname", fullname);
                    cmd.Parameters.AddWithValue("@Sex", sex);
                    cmd.Parameters.AddWithValue("@Password", password);
                    cmd.Parameters.AddWithValue("@ConfirmedPassword", confirmedPassword);
                    cmd.Parameters.AddWithValue("@UserId", userId);

                    var parm = cmd.CreateParameter();
                    parm.ParameterName = "@Err";
                    parm.Direction = ParameterDirection.Output;
                    parm.DbType = DbType.String;
                    parm.Size = 100;
                    cmd.Parameters.Add(parm);

                    cnn.Open();
                    cmd.ExecuteNonQuery();

                    result = parm.Value?.ToString() ?? result;
                }
            }
            catch (Exception)
            {
                result = "DB Error. More info, please contact the admin user.";
            }
            return result;
        }

        public string UserDelete(int id, int userId)
        {
            var result = "Invalid Data input, please try again.";
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserDelete", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@Id", id);
                    cmd.Parameters.AddWithValue("@UserId", userId);

                    var parm = cmd.CreateParameter();
                    parm.ParameterName = "@Err";
                    parm.Direction = ParameterDirection.Output;
                    parm.DbType = DbType.String;
                    parm.Size = 100;
                    cmd.Parameters.Add(parm);

                    cnn.Open();
                    cmd.ExecuteNonQuery();

                    result = parm.Value?.ToString() ?? result;
                }
            }
            catch (Exception)
            {
                result = "DB Error. More info, please contact the admin user.";
            }
            return result;
        }

        public string UserChange(int id, int typeId, string fullname, string sex, int userId)
        {
            var result = "Invalid Data input, please try again.";
            try
            {
                using (var cnn = new SqlConnection(_newConnection.ConnString))
                using (var cmd = new SqlCommand("UserInfoChange", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@Id", id);
                    cmd.Parameters.AddWithValue("@TypeId", typeId);
                    cmd.Parameters.AddWithValue("@Fullname", fullname);
                    cmd.Parameters.AddWithValue("@Sex", sex);
                    cmd.Parameters.AddWithValue("@UserId", userId);

                    var parm = cmd.CreateParameter();
                    parm.ParameterName = "@Err";
                    parm.Direction = ParameterDirection.Output;
                    parm.DbType = DbType.String;
                    parm.Size = 100;
                    cmd.Parameters.Add(parm);

                    cnn.Open();
                    cmd.ExecuteNonQuery();

                    result = parm.Value?.ToString() ?? result;
                }
            }
            catch (Exception)
            {
                result = "DB Error. More info, please contact the admin user.";
            }
            return result;
        }
    }

    // Add this class to fix CS0246 if you do not have a Connection class defined elsewhere.
    // If you already have a Connection class, ensure it is in the correct namespace and referenced properly.
    public class Connection
    {
        public string ConnString { get; }

        public Connection(string connectionString)
        {
            ConnString = connectionString;
        }
    }
}