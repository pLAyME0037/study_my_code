using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.Vehical
{
    internal class Car2 : Vehical2
    {
        public int numberOfWheel;
        public void start()
        {
            Console.WriteLine("Car is starting...");
        }
        public void stop()
        {
            Console.WriteLine("Car is stoping...");
        }
    }
}
