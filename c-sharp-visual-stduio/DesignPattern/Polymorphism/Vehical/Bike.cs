using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.Vehical
{
    internal class Bike2 : Vehical2
    {
        public int numberOfSeat;
        public void start()
        {
            Console.WriteLine("Bike is starting...");
        }
        public void stop()
        {
            Console.WriteLine("Bike is stoping...");
        }
    }
}
