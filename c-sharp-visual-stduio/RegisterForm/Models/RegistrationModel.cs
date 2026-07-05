Models\RegistrationModel.cs
using System;
using System.Data;
using System.Data.SqlClient;

namespace DesktopApp.Models
{
    public class RegistrationModel
    {
        private readonly Connection _connection = new Connection();

        public string AddRegistration(string studentCode, string name, string sex, DateTime dob, int userId)
        {
            var result = "Invalid data input, please try again.";
            try
            {
                using (var cnn = new SqlConnection(_connection.ConnString))
                using (var cmd = new SqlCommand("RegistrationAdd", cnn) { CommandType = CommandType.StoredProcedure })
                {
                    cmd.Parameters.AddWithValue("@StudentCode", studentCode ?? "");
                    cmd.Parameters.AddWithValue("@Name", name ?? "");
                    cmd.Parameters.AddWithValue("@Sex", sex ?? "");
                    cmd.Parameters.AddWithValue("@DoB", dob);
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
}