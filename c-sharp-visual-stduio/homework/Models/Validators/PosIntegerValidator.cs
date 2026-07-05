using System;
using System.Globalization;

namespace Homework.Models.Validators
{
    internal class PosIntegerValidator : IntegerValidator
    {
        public PosIntegerValidator(CultureInfo culture) : base(culture) { }

        public override bool TryValidate(
            string input,
            out int value,
            out string formattedValue
        ) {
            value = 0;
            formattedValue = null;

            if (string.IsNullOrWhiteSpace(input)) return false;

            input = input.Trim();

            // Allow thousands separators and leading sign; culture-aware parsing rejects decimals
            var styles = NumberStyles.Integer | NumberStyles.AllowThousands;

            if (!int.TryParse(input, styles, Culture, out value)) return false;

            // Must be strictly positive
            if (value <= 0) return false;

            // Format using culture (no decimals)
            formattedValue = value.ToString("N0", Culture);
            return true;
        }

        // Returns a culture-aware example string for display/help text.
        public string GetExample()
        {
            // Use group separator if it's commonly shown (e.g. "1,234")
            var groupSep = Culture.NumberFormat.NumberGroupSeparator;
            return $"e.g. 34{groupSep}0 or 102";
        }
    }
}
