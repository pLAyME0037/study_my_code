using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.PaymentSys
{
    public class PayPalProcessor : IPaymentProcessor
    {
        public void ProcessPayment(decimal amount)
        {
            Console.WriteLine($"Processing PayPal payment of ${amount:F2} via secure redirect.");
            // Simulate OAuth flow, redirect, confirmation
        }
    }
}
