namespace ConsoleApp1.Day1;

public class Car
{
    private string name;
    private int year;
    public void setCarType(string name, int year)
    {
        this.name = name;
        this.year = year;
    }

    public void getCarType()
    {
        Console.Write(name + ", ");
        Console.WriteLine(year);
    }
}
