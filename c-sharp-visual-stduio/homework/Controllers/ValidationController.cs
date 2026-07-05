using Homework.Models.Validators;
using Homework.Views.Interfaces;
using System.Globalization;

namespace Homework.Controllers
{
    public class ValidationController
    {
        private readonly IValidationView _view;
        public ValidationController(IValidationView view) => _view = view;

        public void ValidateAndDisplay(DecimalValidator validator, string ruleDescription)
        {
            string input = _view.GetInput().Trim();

            if (validator.TryValidate(
                input,
                out decimal validatedValue,
                out string formattedValue
            ))
            {
                _view.SetInput(formattedValue);
                _view.SetResultText($"Valid decimal: {formattedValue}");
                _view.ShowInfo($"Valid decimal: {formattedValue}", "Validation Success");
            }
            else
            {
                string separator = CultureInfo
                    .CurrentCulture
                    .NumberFormat
                    .NumberDecimalSeparator;
                string example = ruleDescription.Contains("positive")
                    ? $"e.g. 34{separator}0 or 34{separator}00"
                    : $"e.g. 34{separator}00 or -12{separator}34";

                _view.SetResultText("Not a valid decimal.");
                _view.ShowWarning(
                    $"Not a valid decimal.\nPlease enter a positive value with {ruleDescription} ({example}).",
                    "Validation Error");
                _view.FocusAndSelectAll();
            }
        }

        public void ValidateAndDisplay(IntegerValidator validator, string ruleDescription)
        {
            string input = _view.GetInput().Trim();

            if (validator.TryValidate(
                input,
                out int validatedValue,
                out string formattedValue
            ))
            {
                _view.SetInput(formattedValue);
                _view.SetResultText($"Valid integer: {formattedValue}");
                _view.ShowInfo($"Valid integer: {formattedValue}", "Validation Success");
            }
            else
            {
                string example = ruleDescription.Contains("positive")
                    ? "e.g. 34 or 102"
                    : "e.g. 34 or -12";

                _view.SetResultText("Not a valid integer.");
                _view.ShowWarning(
                    $"Not a valid integer.\nPlease enter a value with {ruleDescription} ({example}).",
                    "Validation Error");
                _view.FocusAndSelectAll();
            }
        }
    }
}