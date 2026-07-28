using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Security;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Abstraction
{
    // Reduce complexity by hiding unnessary detail.
    internal class EmailService
    {
        public void sendEmail()
        {
            connect();
            authenticate();
            Console.WriteLine("Sending Email...");
            disconnect();
        }

        private void disconnect()
        {
            Console.WriteLine("Disconnecting to Email server...");
        }

        private void authenticate()
        {
            Console.WriteLine("authenticating...");
        }

        private void connect()
        {
            Console.WriteLine("Connecting to Email server...");
        }
    }
}
