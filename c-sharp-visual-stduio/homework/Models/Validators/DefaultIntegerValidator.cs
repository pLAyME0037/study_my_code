using System;
using System.Globalization;

namespace Homework.Models.Validators
{
    public class DefaultIntegerValidator : IntegerValidator
    {
        public DefaultIntegerValidator(CultureInfo culture) : base(culture) { }

        public override bool TryValidate(
            string input, 
            out int value, 
            out string formattedValue
        ) {
            value = 0;
            formattedValue = null;

            if (string.IsNullOrWhiteSpace(input)) return false;

            // Allow thousands separators and leading sign; culture-aware parsing rejects decimals
            var styles = NumberStyles.Integer | NumberStyles.AllowThousands;

            if (!int.TryParse(input, styles, Culture, out value)) return false;

            // Format using culture (no decimals)
            formattedValue = value.ToString("N0", Culture);
            return true;
        }
    }
}