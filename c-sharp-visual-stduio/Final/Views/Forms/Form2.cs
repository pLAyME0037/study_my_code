using Final.Helpers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Final.Views.Forms
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            this.Text = "Welcom to Form 2";
        }

        private void btCloseFm_Click(object sender, EventArgs e)
        {
            NavigationManager.NavigateBack(this);
        }

        private void btCloseApp_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
