using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesktopApp.Models
{
    internal class SexComboBox
    {
        public string _sex = null;

        public SexComboBox(string sex) { 
            this._sex = sex;
        }

        public override string ToString()
        {
            return this._sex.ToString();
        }
    }
}
