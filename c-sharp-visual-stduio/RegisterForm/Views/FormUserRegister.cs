using DesktopApp.Models;
using System;
using System.Windows.Forms;
using System.Xml.Linq;

namespace DesktopApp.Forms
{
    public partial class FormUserRegister : Form
    {
        private readonly string _connectionString;

        public FormUserRegister()
        {
            InitializeComponent();
            // wire up the Add button click
            this.button1.Click += Button1_Click;

            // Load connection string from XML file
            _connectionString = LoadConnectionString();
        }

        private string LoadConnectionString()
        {
            try
            {
                var doc = XDocument.Load("connection.xml");
                var serverAddress = doc.Root.Element("serveraddress")?.Value;
                var dbName = doc.Root.Element("db_name")?.Value;

                return $"Server={serverAddress};Database={dbName};Trusted_Connection=True;";
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading connection string: {ex.Message}");
                return string.Empty;
            }
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            var studentCode = textBox1.Text.Trim();
            var name = textBox4.Text.Trim();
            var sex = textBox3.Text.Trim();
            var dob = dateTimePicker1.Value;

            var reg = new UserModel(_connectionString);

            // NOTE: Pass the current user id; for now we use 1 as placeholder.
            var result = reg.UserAdd(1, studentCode, name, sex, "", "", 1);

            MessageBox.Show(result, "Registration", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }                   
    }
}
