using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Encasulation
{
    public class BankAccount
    {
        private decimal balance;

        public decimal getBalance()
        {
            return balance;
        }
        public BankAccount(decimal balance)
        {
            deposit(balance);
        }
        public void deposit(decimal amount)
        {
            if (amount <= 0)
            {
                throw new ArgumentException("Deposit amount must be positive.");
            }
            this.balance += amount;
        }
        public void withdraw(decimal amount)
        {
            if (amount <= 0)
            {
                throw new ArgumentException("Withdraw amount must be postive.");
            }
            if (amount > balance)
            {
                throw new InvalidOperationException("Insufficient funding.");
            }
            this.balance -= amount;
        }
    }
}
