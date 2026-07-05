using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RegistrationApp.MyTools
{
    class mycombobox
    {
        public string Name;
        public int Value;
        public mycombobox(string name, int value)
        {
            Name = name;
            Value = value;
        }
        public override string ToString()
        {
            // Generates the text shown in the combo box
            return Name;
        }
    }
}
