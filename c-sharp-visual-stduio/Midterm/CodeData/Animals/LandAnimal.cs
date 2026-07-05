using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Midterm.CodeData.Animals;
internal class LandAnimal : Animal
{
    private string _name = "";
    private decimal _runSpeed = 0;
    private string _scale = "meter";

    public void AnimalRunSpeed(string name, decimal runSpeed, string scale)
    {
        this._name = name;
        this._runSpeed = runSpeed;
        this._scale = scale;
        Console.WriteLine(_name + " can run " + _runSpeed + _scale + " Per seconed");
    }
}
