using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPattern.Polymorpishism
{
    internal class Vehical
    {
        public string _brand {  get; set; }
        public string _name { get; set; }
        public int _year { get; set; }

        public static void start()
        {
            Console.WriteLine("Vehical is starting...");
        }
        public static void stop()
        {
            Console.WriteLine("Vehical is stoping...");
        }
    }
}
