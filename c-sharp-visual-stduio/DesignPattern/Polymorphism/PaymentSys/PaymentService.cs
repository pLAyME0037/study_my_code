using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.PaymentSys
{
    // 3. Context that uses the strategy (polymorphic behavior)
    public class PaymentService
    {
        private readonly IPaymentProcessor _processor;

        // Dependency injection via constructor
        public PaymentService(IPaymentProcessor processor)
        {
            _processor = processor ?? throw new ArgumentNullException(nameof(processor));
        }

        public void ProcessOrderPayment(decimal orderTotal)
        {
            Console.WriteLine("Starting payment process...");
            _processor.ProcessPayment(orderTotal); // Polymorphic call
            Console.WriteLine("Payment completed.\n");
        }
    }
}
