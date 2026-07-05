using RegistrationApp.Views;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RegistrationApp
{
    public partial class FmDashboard : Form
    {
        public int UserId { get; set; }
       
        public FmDashboard()
        {
            InitializeComponent();
        }

        private void registrationAddToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FmRegistrationMng FmReMng = new FmRegistrationMng();
            FmReMng.MdiParent = this;
            FmReMng.UserId = UserId;
            FmReMng.Show();
        }

        private void FmDashboard_FormClosing(object sender, FormClosingEventArgs e)
        {
            Application.Exit();
        }

        private void FmDashboard_Load(object sender, EventArgs e)
        {

        }

        private void toolStripDropDownButton1_Click(object sender, EventArgs e)
        {

        }

        private void toolStripDropDownButton2_Click(object sender, EventArgs e)
        {
            
        }

        private void tEstToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FmUserManagement FmMng = new FmUserManagement();
            FmMng.MdiParent = this;
            FmMng.UserId = UserId;
            FmMng.Show();

        }

        private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void toolStripDropDownButton2_Click_1(object sender, EventArgs e)
        {

        }

        private void registrationManagementToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FmRegistrationMng FmMng = new FmRegistrationMng();
            FmMng.MdiParent = this;
            FmMng.UserId = UserId;
            FmMng.Show();
        }

        private void FmDashboard_KeyDown(object sender, KeyEventArgs e)
        {
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ValidationForm validationForm = new ValidationForm();
            validationForm.MdiParent = this;
            validationForm.Show();
        }
    }
}
