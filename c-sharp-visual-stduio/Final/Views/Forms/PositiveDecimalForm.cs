using Final.Controllers;
using Final.Helpers;
using Final.Models;
using Final.Views.Interfaces;
using System;
using System.Globalization;
using System.Windows.Forms;

namespace Final.Views.Forms
{
    public partial class PositiveDecimalForm : Form, IValidationView
    {
        private readonly DecimalValidator _validator;
        private readonly ValidationController _controller;

        public PositiveDecimalForm()
        {
            InitializeComponent();

            var culture = CultureInfo.CurrentCulture;
            _validator = new PositiveDecimalValidator(culture);

            _controller = new ValidationController(this);
        }

        private void btOutPut_Click(object sender, EventArgs e)
        {
            _controller.ValidateAndDisplay(_validator, "positive decimal");
        }

        // IValidationView implementation
        public string GetInput() => textBox1.Text ?? string.Empty;

        public void SetInput(string text)
        {
            textBox1.Text = text ?? string.Empty;
        }

        public void SetResultText(string text)
        {
            textBox2.Text = text ?? string.Empty;
        }

        public void ShowInfo(string message, string caption)
        {
            MessageBox.Show(this, message, caption, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        public void ShowWarning(string message, string caption)
        {
            MessageBox.Show(this, message, caption, MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }

        public void FocusAndSelectAll()
        {
            textBox1.Focus();
            textBox1.SelectAll();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            NavigationManager.NavigateBack(this);
        }
    }
}
