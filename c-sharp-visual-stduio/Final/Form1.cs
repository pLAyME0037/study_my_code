using Final.Helpers;
using Final.Models;
using Final.Views.Forms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Final
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void btToFm2_Click(object sender, EventArgs e)
        {
            var form2 = new Form2();
            NavigationManager.Navigate(this, form2);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = "Hello, Welcom to Form 1";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var deciVar = new PositiveDecimalForm();
            NavigationManager.Navigate(this, deciVar);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var calCommForm = new Commision();
            NavigationManager.Navigate(this, calCommForm);
        }
    }
}
