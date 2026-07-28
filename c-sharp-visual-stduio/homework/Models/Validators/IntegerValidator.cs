using System;
using System.Globalization;

namespace Homework.Models.Validators
{
    public abstract class IntegerValidator
    {
        protected CultureInfo Culture { get; }

        protected IntegerValidator(CultureInfo culture)
        {
            Culture = culture ?? throw new ArgumentNullException(nameof(culture));
        }

        public abstract bool TryValidate(
            string input, 
            out int value, 
            out string formattedValue
        );

        protected string GetIntegerSeparator() => String.Empty;
    }
}
