namespace ConsoleApp1.Day1
{
    internal class User
    {
        public string name = "";
        public int age = 0;

        public void UserInfo(string name, int age)
        {
            this.name = name;
            this.age = age;

            Console.WriteLine("Your Name is : " + this.name);
            Console.WriteLine("Your Age is : " + this.age);
        }
    }
}
