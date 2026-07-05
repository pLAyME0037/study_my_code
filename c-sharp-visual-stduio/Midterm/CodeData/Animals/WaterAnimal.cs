using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Midterm.CodeData.Animals;
internal class WaterAnimal : Animal
{
    private string _name = "";
    private int _swim = 0;
    private string _scale = "meter";

    public void AnimalSwimSpeed(string name, int runSpeed, string scale)
    {
        this._name = name;
        this._swim = runSpeed;
        this._scale = scale;

        Console.WriteLine(_name + " can swim " + _swim + _scale + " Per seconed");
    }
}
