using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Midterm.CodeData.Animals;
public class Animal
{
    public string Name { get; set; } = "null";
    public string Description { get; set; } = "null";

    public void AnimalInfo(string name, string description)
    {
        Name = name;
        Description = description;

        Console.WriteLine("All " + Name + " can " + Description);
    }
}
