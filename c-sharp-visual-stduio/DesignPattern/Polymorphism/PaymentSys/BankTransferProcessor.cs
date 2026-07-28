using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.PaymentSys
{
    public class BankTransferProcessor : IPaymentProcessor
    {
        public void ProcessPayment(decimal amount)
        {
            Console.WriteLine($"Initiating bank transfer of ${amount:F2}. ACH processing may take 1-3 days.");
        }
    }
}
