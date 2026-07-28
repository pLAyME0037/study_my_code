using RegistrationApp.Models;
using RegistrationApp.Views;
using System;
using System.Windows.Forms;
using System.Diagnostics;

namespace RegistrationApp
{
    public partial class FmLogin : Form
    {
        public FmLogin()
        {
            InitializeComponent();
        }

        private void BtExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void BtLogin_Click(object sender, EventArgs e)
        {
            // Diagnostic: confirm click fired
            MessageBox.Show("BtLogin_Click fired", "Diagnostic");

            // Call UserLogin once and store the result
            UserModel userMd = new UserModel();
            string loginResult = userMd.UserLogin(TxtUsername.Text, TxtPassword.Text);

            // Log return value for troubleshooting
            Debug.WriteLine($"UserLogin returned: '{loginResult ?? "<null>"}'");

            // Treat null or empty as success
            if (string.IsNullOrEmpty(loginResult))
            {
                // Show ValidationForm modally. Hide this form to ensure visibility.
                using (var validationForm = new ValidationForm())
                {
                    this.Hide();
                    validationForm.ShowDialog();
                    this.Show();
                }
            }
            else
            {
                MessageBox.Show(loginResult, "Information");
            }
        }

        private void FmLogin_Load(object sender, EventArgs e)
        {
        }
    }
}
