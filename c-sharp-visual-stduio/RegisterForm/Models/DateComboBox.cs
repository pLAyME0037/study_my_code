using System;

// No changes needed in this file for CS0122 after making _name public in MyComboBox.
using System.Collections.Generic;
using System.Linq;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;

namespace DesktopApp.Models
{
    internal class DateComboBox
    {
        public string _name = null;
        public string _value = null;

        public DateComboBox(string name, string value)
        {
            _name = name;
            _value = value;
        }

        public override string ToString()
        {
            return _name;
        }
    }
}
