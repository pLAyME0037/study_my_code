using ConsoleApp1.Day1;
using ConsoleApp1.Day2;
using ConsoleApp1.Day3;
using System;

namespace ConsoleApp1
{
    class Program
    {
        public static void Main()
        {
            while (true)
            {
                Console.WriteLine();
                Console.WriteLine("Select index to run:");
                Console.WriteLine("Choose 1 - 8");
                Console.WriteLine(" 0 - Exit");
                Console.Write("Enter index: ");

                string? input = Console.ReadLine();
                if (!int.TryParse(input, out int index))
                {
                    Console.WriteLine("Invalid input. Please enter a number.");
                    continue;
                }

                if (index == 0)
                {
                    Console.WriteLine("Exiting...");
                    break;
                }

                switch (index)
                {
                    case 1:
                        {
                            Caluate caluate = new Caluate();
                            Console.WriteLine(caluate.Sum(2, 3));
                            Console.WriteLine(caluate.Multi(3, 9));
                            break;
                        }
                    case 2:
                        {
                            Car car = new Car();
                            car.setCarType("aa", 2000);
                            car.getCarType();
                            break;
                        }
                    case 3:
                        {
                            User user = new User();
                            user.UserInfo("Mean", 21);
                            break;
                        }
                    case 4:
                        {
                            IfUsage ifUsage = new IfUsage();
                            ifUsage.ConvertToDay(1);
                            break;
                        }
                    case 5:
                        {
                            Student student = new Student();
                            student.SetSudentInfo();
                            student.studentAdd("Mean");
                            student.studentDelete(3);
                            break;
                        } 
                    case 6:
                        {
                            UseParam useParam = new UseParam();
                            useParam.Sum(2, 4, 4, 5, 44, 22, 24);
                            useParam.ConcatString("Hello,", "World", "!");
                            useParam.ConcatStrUseForeach("Hello,", "World", "!");
                            break;
                        }
                    case 7:
                        {
                            // Run Car1 behavior
                            Car1 car = new();
                            car.activateKey();
                            car.Honk();
                            Console.WriteLine($"Car: model = {car.modelName}, brand = {car.brand}");
                            break;
                        }
                    case 8:
                        {
                            // Run Vehical behavior
                            Vehical veh = new();
                            veh.activateKey();
                            veh.Honk();
                            Console.WriteLine($"Vehical: brand = {veh.brand}");
                            break;
                        }
                    default:
                        Console.WriteLine($"No action mapped for index {index}. Add a case in Program.Main to handle it.");
                        break;
                }
            }
        }
    }
}