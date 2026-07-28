using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Midterm.CodeData;
internal class UserInfo
{
    private string _name = null;
    private int _age = 0;

    public UserInfo()
    {
        Console.WriteLine("==== User Infomaation ====");
    }
    public void setUserInfo(string name, int age)
    {
        this._name = name;
        this._age = age;
    }

    public void getUserInfo()
    {
        Console.WriteLine("Hello," + this._name);
        Console.WriteLine("You are " + this._age + " years old.");
    }
}
