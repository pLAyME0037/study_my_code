using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace Final.Models
{
    internal class PositiveDecimalValidator : DecimalValidator
    {
        public PositiveDecimalValidator(CultureInfo culture) : base(culture) { }
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

            formattedValue = value.ToString("G29", Culture);
            return true;
        }

    }
}
