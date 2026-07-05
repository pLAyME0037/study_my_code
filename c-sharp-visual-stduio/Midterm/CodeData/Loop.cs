using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Midterm.CodeData;
internal class Loop
{

    public int Sum(params int[] nums)
    {
        int total = 0;
        foreach (int i in nums)
        {
            total += i;
        }
        return total;
    }

    public int[] ValidateNumber(string[] nums)
    {
        int[] results = new int[nums.Length];

        for (int i = 0; i < nums.Length; i++)
        {
            string s = nums[i];
            if (int.TryParse(s, out int parsed))
            {
                results[i] = parsed;
            }
            else
            {
                throw new FormatException($"Input '{s}' at index {i} is not a valid integer.");
            }
        }
        return results;
    }

    // Pseudocode:
    // 1. If 'numbers' is null -> throw ArgumentNullException
    // 2. Determine separators: use provided 'separators' if any, otherwise use default set
    // 3. Split 'numbers' into parts, removing empty entries
    // 4. If no parts -> return 0
    // 5. Initialize total = 0
    // 6. For each part with index i:
    //    a. Use Convert.ToInt32(part) to parse (this throws on invalid format/overflow)
    //    b. Catch FormatException and OverflowException to throw a more informative exception including index and value
    //    c. Add parsed value to total
    // 7. Return total
        public int Sum(string numbers, params char[] separators)
        {
            if (numbers is null)
                throw new ArgumentNullException(nameof(numbers));

            char[] defaultSeparators = new[] { ',', ';', ' ', '\n', '\r', '\t' };
            char[] seps = (separators != null && separators.Length > 0) ? separators : defaultSeparators;

            string[] parts = numbers.Split(seps, StringSplitOptions.RemoveEmptyEntries);
            if (parts.Length == 0)
                return 0;

            int total = 0;
            for (int i = 0; i < parts.Length; i++)
            {
                string s = parts[i];
                try
                {
                    int value = Convert.ToInt32(s);
                    total += value;
                }
                catch (FormatException)
                {
                    throw new FormatException($"Input '{s}' at index {i} is not a valid integer.");
                }
                catch (OverflowException)
                {
                    throw new OverflowException($"Input '{s}' at index {i} is outside the range of an Int32.");
                }
            }

            return total;
        }
}
