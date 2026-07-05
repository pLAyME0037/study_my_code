using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics.Eventing.Reader;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DesktopApp.Forms
{
    public partial class FormRadio : Form
    {
        private string _selectedRadio = string.Empty;
        public FormRadio()
        {
            InitializeComponent();
        }

        private void FormRadio_Load(object sender, EventArgs e)
        {
            this.Text = "Radio";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(radioButton1.Checked)
            {
                this._selectedRadio = radioButton1.Text;
            }
            else if (radioButton2.Checked) 
            {
               this._selectedRadio = radioButton2.Text;
            }
            else if (radioButton3.Checked)
            {
                this._selectedRadio = radioButton3.Text;
            }
            else if (radioButton4.Checked)
            {
                this._selectedRadio = radioButton4.Text;
            }

            MessageBox.Show(this._selectedRadio);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            foreach (Control control in this.groupBox1.Controls)
            {
                if (control is RadioButton radioButton && radioButton.Checked)
                {
                this._selectedRadio = radioButton.Text;
                break;
                }
                MessageBox.Show(_selectedRadio);
            }
        }
    }
}
