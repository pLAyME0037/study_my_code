using System;
using System.Windows.Forms;

namespace RegistrationApp.Views
{
    public partial class ValidationForm : Form
    {
        public ValidationForm()
        {
            InitializeComponent();
        }

        private void Validation_Load(object sender, EventArgs e)
        {

        }

        private void btCheckDecimal_Click(object sender, EventArgs e)
        {
            decimal __data;
            string input = this.textBoxData.Text.Trim();

            bool isDecimal = decimal.TryParse(input, out __data);

            if (isDecimal)
            {
                this.textBoxData.Text = __data.ToString();
                MessageBox.Show($"Valid decimal: {__data}", "Validation", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show("Please enter a valid decimal number.", "Validation error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                this.textBoxData.Focus();
                this.textBoxData.SelectAll();
            }
        }
    }
}
