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

namespace MainForm.Views.Forms
{
    public partial class StudentScore : Form
    {
        private Student student;
        public StudentScore()
        {
            InitializeComponent();
            student = new Student();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.Text = "Student Grade";
        }

        private void clearResult()
        {
            this.textBoxScore.Text = "";
            this.textBoxGrade.Text = "";
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if (float.TryParse(textBoxScore.Text, out float score))
            {
                if (score >= 0 && score <= 100)
                {
                    string grade = student.CalculateGrade(score);
                    this.textBoxGrade.Text = grade;
                }
                else if (score < 0)
                {
                    MessageBox.Show(
                        "Score cannot be negative! Please enter a score between 0 and 100.",
                        "Error",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error
                    );
                    textBoxScore.Focus();
                    textBoxScore.SelectAll();
                    clearResult();
                }
                else // score > 100
                {
                    MessageBox.Show(
                        "Score cannot exceed 100! Please enter a score between 0 and 100.",
                        "Error",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error
                    );
                    textBoxScore.Focus();
                    textBoxScore.SelectAll();
                    clearResult();
                }
            }
            else
            {
                MessageBox.Show(
                    "Please enter a valid numeric score!",
                    "Input Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                );
                textBoxScore.Focus();
                textBoxScore.SelectAll();
                clearResult();
            }
        }

        private void StudentScore_Load(object sender, EventArgs e)
        {
            this.Text = "Score";
        }
    }
}
