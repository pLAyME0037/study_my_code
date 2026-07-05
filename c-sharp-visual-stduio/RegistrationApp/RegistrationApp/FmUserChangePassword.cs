using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using RegistrationApp.Models;

namespace RegistrationApp
{
    public partial class FmUserChangePassword : Form
    {
        public int Id { get; set; }
        public int UserId { get; set; }
        public FmUserChangePassword()
        {
            InitializeComponent();
        }

        private void FmUserChangePassword_Load(object sender, EventArgs e)
        {

        }

        private void BtChange_Click(object sender, EventArgs e)
        {
            UserModel UserMd = new UserModel();

            MessageBox.Show(UserMd.UserChangePassword(Id,TxtNewPassword.Text,TxtNewConfirmedPassword.Text,TxtPassword.Text,UserId));
        }
    }
}
