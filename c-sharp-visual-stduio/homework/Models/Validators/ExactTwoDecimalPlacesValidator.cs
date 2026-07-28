using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Homework.Models.Validators
{
    // Requires exactly two decimal places (e.g., 123.45, -0.50)
    public class ExactTwoDecimalPlacesValidator : DecimalValidator
    {
        private readonly string _pattern;

        public ExactTwoDecimalPlacesValidator(CultureInfo culture) : base(culture)
        {
            string sep = Regex.Escape(GetDecimalSeparator());
            _pattern = $"^-?\\d+{sep}\\d{{2}}$";
        }

        public override bool TryValidate(string input, out decimal value, out string formattedValue)
        {
            value = 0m;
            formattedValue = null;

            if (string.IsNullOrWhiteSpace(input))
                return false;

            bool formatMatches = Regex.IsMatch(input, _pattern);

            if (
                !formatMatches || 
                !decimal.TryParse(input, NumberStyles.Number, Culture, out value)
            ) return false;

            formattedValue = value.ToString("F2", Culture);
            return true;
        }
    }
}
