using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1.Day3
{
    class Vehical
    {
        public string brand = "Ford";
        public void Honk()
        {
            Console.WriteLine("beep beep");
        }

        public void activateKey()
        {
            start();
            stop();
        }
        public void start()
        {
            Console.WriteLine("Vehical is started");
        }
        public void stop()
        {
            Console.WriteLine("Vehical is stoped");
        }
    }
}
