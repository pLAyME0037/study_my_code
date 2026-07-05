using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Homework.Models
{
    public class UsingConvertTo
    {
        // Direct Convert.ToX implementations (may throw)
        public int ConvertToInt(string input) => Convert.ToInt32(input);

        public float ConvertToFloat(string input) => Convert.ToSingle(input);

        public decimal ConvertToDecimal(string input) => Convert.ToDecimal(input);

        public string ConvertToString(object input) => Convert.ToString(input);

        public char ConvertToChar(string input) => Convert.ToChar(input);

        public bool ConvertToBool(string input) => Convert.ToBoolean(input);

        // Safer Try-patterns using TryParse where applicable
        public bool TryConvertToInt(string input, out int value) => int.TryParse(input, out value);

        public bool TryConvertToFloat(string input, out float value) => float.TryParse(input, out value);

        public bool TryConvertToDecimal(string input, out decimal value) => decimal.TryParse(input, out value);

        public bool TryConvertToChar(string input, out char value)
        {
            value = default(char);
            if (string.IsNullOrEmpty(input) || input.Length != 1) return false;
            value = input[0];
            return true;
        }

        public bool TryConvertToBool(string input, out bool value) => bool.TryParse(input, out value);
    }
}
