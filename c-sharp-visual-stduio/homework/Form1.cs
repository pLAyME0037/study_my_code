using Homework.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Homework.Views.Forms;
using MainForm.Views.Forms;
using Homework.Helpers;

namespace MainForm
{
    public partial class Form1 : Form
    {
        private StudentScore studentScore = new StudentScore();
        private UsingConvertToForm usingConvertToForm = new UsingConvertToForm();
        //private StoreForm storeForm = new StoreForm();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = "Main Form";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.studentScore.ShowDialog();
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            this.usingConvertToForm.ShowDialog();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //this.storeForm.ShowDialog();
            var storeForm = new StoreForm();
            NavigationManager.Navigate(this, storeForm);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            // create new target form when navigating (avoids stale state and follows DRY)
            var validationForm = new ValidationForm();
            NavigationManager.Navigate(this, validationForm);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
