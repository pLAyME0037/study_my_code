using DesignPattern.Polymorphism.PaymentSys;

IPaymentProcessor creditCard = new CreditCardProcessor();
IPaymentProcessor paypal = new PayPalProcessor();
IPaymentProcessor bankTransfer = new BankTransferProcessor();

var paymentService = new PaymentService(creditCard);
paymentService.ProcessOrderPayment(99.99m);

paymentService = new PaymentService(paypal);
paymentService.ProcessOrderPayment(149.50m);

paymentService = new PaymentService(bankTransfer);
paymentService.ProcessOrderPayment(250.00m);

paymentService = new PaymentService(paypal);
paymentService.ProcessOrderPayment(345);
