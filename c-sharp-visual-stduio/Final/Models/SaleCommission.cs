using System;

namespace Final.Models
{
    internal class SaleCommission
    {
        public decimal CalCommision(decimal amount)
        {
            // Use original amount and compute commission into a separate variable.
            decimal commission = 0m;

            if (amount >= 6000000m)
            {
                commission = amount * 0.10m;
            }
            else if (amount >= 3000000m)
            {
                commission = amount * 0.05m;
            }
            else if (amount >= 1500000m)
            {
                commission = amount * 0.03m;
            }
            else
            {
                commission = amount * 0.01m;
            }

            return commission;
        }
    }
}
