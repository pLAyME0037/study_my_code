using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DesktopApp.Forms
{
    public partial class FormDashboard : Form
    {
        public int userId {  get; set; }
        public FormDashboard()
        {
            InitializeComponent();
        }

        private void FormDashboard_Load(object sender, EventArgs e)
        {
            this.Text = "Dashboard";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(this.userId);
        }

        private void rigisterManagementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormRegister registerForm = new FormRegister();
            registerForm.ShowDialog();
        }

        private void userToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormUser userForm = new FormUser();
            userForm.ShowDialog();
        }

        private void radioToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormRadio radioForm = new FormRadio();
            radioForm.ShowDialog();
        }

        private void checkboxToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormCheckBox checkBox = new FormCheckBox();
            checkBox.ShowDialog();
        }
    }
}
