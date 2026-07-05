using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Homework.Models.Validators
{
    // Requires positive number with 1 or 2 decimal places (e.g., 10.5 or 10.50)
    public class PositiveOneOrTwoDecimalPlacesValidator : DecimalValidator
    {
        public PositiveOneOrTwoDecimalPlacesValidator(CultureInfo culture) : base(culture) { }

        public override bool TryValidate(string input, out decimal value, out string formattedValue)
        {
            value = 0m;
            formattedValue = null;

            if (string.IsNullOrWhiteSpace(input))
                return false;

            if (
                !decimal.TryParse(input, NumberStyles.Number, Culture, out value) 
                || value < 0m
            ) return false;

            // Extract scale (number of decimal places)
            int scale = (decimal.GetBits(value)[3] >> 16) & 0xFF;

            if (scale != 1 && scale != 2)
                return false;

            formattedValue = value.ToString("F2", Culture);
            return true;
        }
    }
}
