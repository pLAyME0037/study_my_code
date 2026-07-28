using Homework.Controllers;
using Homework.Helpers;
using Homework.Models.Validators;
using Homework.Views.Interfaces;
using System;
using System.Globalization;
using System.Windows.Forms;

namespace Homework.Views.Forms
{
    public partial class ValidationForm : Form, IValidationView
    {
        private readonly DecimalValidator _exactTwoDecimalsValidator;
        private readonly DecimalValidator _posDecimalsValidator;
        private readonly DecimalValidator _defDecimalsValidator;
        private readonly IntegerValidator _integerValidator;
        private readonly IntegerValidator _posIntegerValidator;

        private readonly ValidationController _controller;

        public ValidationForm()
        {
            InitializeComponent();

            var culture = CultureInfo.CurrentCulture;
            _exactTwoDecimalsValidator = new ExactTwoDecimalPlacesValidator(culture);
            _posDecimalsValidator = new PositiveOneOrTwoDecimalPlacesValidator(culture);
            _defDecimalsValidator = new DefaultDecimalValidator(culture);
            _integerValidator = new DefaultIntegerValidator(culture);
            _posIntegerValidator = new PosIntegerValidator(culture);

            _controller = new ValidationController(this);
        }

        private void ValidationForm_Load(object sender, EventArgs e)
        {
            textBoxData.Focus();
        }

        // IValidationView implementations
        public string GetInput() => textBoxData.Text;
        public void SetInput(string text) => textBoxData.Text = text;
        public void SetResultText(string text) => textBoxResult.Text = text;
        public void ShowInfo(string message, string caption) => MessageBox.Show(
            message, 
            caption, 
            MessageBoxButtons.OK, 
            MessageBoxIcon.Information
            );
        public void ShowWarning(string message, string caption) => MessageBox.Show(
            message, 
            caption, 
            MessageBoxButtons.OK, 
            MessageBoxIcon.Warning
            );
        public void FocusAndSelectAll() { textBoxData.Focus(); textBoxData.SelectAll(); }

        // Button handlers now delegate to the controller
        private void btCheckDecimal_Click(object sender, EventArgs e)
        {
            _controller.ValidateAndDisplay(
                _exactTwoDecimalsValidator, 
                "exactly two decimal places"
            );
        }
                                                                                
        private void btCheckPosDecimal_Click(object sender, EventArgs e)
        {
            _controller.ValidateAndDisplay(
                _posDecimalsValidator, 
                "one or two decimal places (positive)"
            );
        }

        private void btCheckInt_Click(object sender, EventArgs e)
        {
            _controller.ValidateAndDisplay(
                _integerValidator, 
                "integer places"
            );
        }

        private void btCheckPosInt_Click(object sender, EventArgs e)
        {
             _controller.ValidateAndDisplay(
                _posIntegerValidator,
                "positive integer places"
            );
        }

        private void ValidationForm_Load_1(object sender, EventArgs e)
        {
            this.Text = "Validation Form";
        }

        private void btBack_Click(object sender, EventArgs e)
        {
            NavigationManager.NavigateBack(this);
        }

        private void btCheckDicimalDef_Click(object sender, EventArgs e)
        {
            _controller.ValidateAndDisplay(_defDecimalsValidator, "decimal places");
        }
    }
}