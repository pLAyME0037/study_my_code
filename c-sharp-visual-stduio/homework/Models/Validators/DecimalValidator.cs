using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Homework.Models.Validators
{
    // Abstract base for validation strategy
    public abstract class DecimalValidator
    {
        protected CultureInfo Culture { get; }

        protected DecimalValidator(CultureInfo culture)
        {
            Culture = culture ?? throw new ArgumentNullException(nameof(culture));
        }

        public abstract bool TryValidate(
            string input, 
            out decimal value, 
            out string formattedValue
            );

        protected string GetDecimalSeparator() => Culture.NumberFormat.NumberDecimalSeparator;
    }
}
