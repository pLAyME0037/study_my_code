using DesktopApp.Models;
using System;
using System.Windows.Forms;

namespace DesktopApp.Forms
{
    public partial class FormUser : Form
    {
        public FormUser()
        {
            InitializeComponent();
        }
        private void userToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormUserRegister userRegisterForm = new FormUserRegister();
            userRegisterForm.ShowDialog();
        }

        private void FormUser_Load(object sender, EventArgs e)
        {
            this.Text = "My ComboBox";

            this.comboBoxFruit.Items.Add("Apple");
            this.comboBoxFruit.Items.Add("Banana");
            this.comboBoxFruit.Items.Add("Cerry");

            this.comboBoxFruit.SelectedIndex = 0;

            this.comboBoxSex.Items.Add("M");
            this.comboBoxSex.Items.Add("F");
            this.comboBoxSex.Items.Add("Other");

            this.comboBoxSex.Text = "Gender Not Selected.";

            this.comboBoxDate.DropDownStyle = ComboBoxStyle.DropDownList;
            this.comboBoxDate.Items.Add(new DateComboBox("Monday", "1"));
            this.comboBoxDate.Items.Add(new DateComboBox("Tuesday", "2"));
            this.comboBoxDate.Items.Add(new DateComboBox("Wednesday", "3"));
            this.comboBoxDate.Items.Add(new DateComboBox("Thursday", "4"));

            this.comboBoxDate.SelectedIndex = 0;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(comboBoxFruit.Text + ", " + comboBoxSex.Text);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string _name = "";
            string _value = "";

            if (comboBoxDate.SelectedItem != null)
            {
                DateComboBox item = comboBoxDate.SelectedItem as DateComboBox;
                if (item != null)
                {
                    _name = item._name;
                    _value = item._value;

                    MessageBox.Show("Date: " + _name + ", Value: " + _value);
                }
            }
        }

    }
}
