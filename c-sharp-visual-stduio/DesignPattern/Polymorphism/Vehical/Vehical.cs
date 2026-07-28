using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorphism.Vehical
{
    internal class Vehical2
    {
        public string _brand {  get; set; }
        public string _name { get; set; }
        public int _year { get; set; }

        public void start()
        {
            Console.WriteLine("Vehical is starting...");
        }
        public void stop()
        {
            Console.WriteLine("Vehical is stoping...");
        }
    }
}
