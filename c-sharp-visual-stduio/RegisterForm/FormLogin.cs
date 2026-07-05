using DesktopApp.Forms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DesktopApp
{
    public partial class FormLogin : Form
    {
        public FormLogin()
        {
            InitializeComponent();
        }

        private void FormLogin_Load(object sender, EventArgs e)
        {
            this.BackColor = Color.Bisque;
            this.WindowState = FormWindowState.Maximized;
            this.Text = "rer";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox2.Focus();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (
                textBox1.Text == "123456"
                && textBox2.Text == "test"
                || textBox1.Text == ""
                && textBox2.Text == ""
            )
            {
                FormDashboard dashboard = new FormDashboard();
                dashboard.userId = 1;
                dashboard.ShowDialog();
                Close();
            }
            else
            {
                MessageBox.Show("Incorrect Username or Password.");
            }
        }
    }
}
