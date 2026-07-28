using Midterm.CodeData;
using Midterm.CodeData.Animals;

UserInfo userInfo = new UserInfo();
userInfo.setUserInfo("Sok Mean", 21);
userInfo.getUserInfo();

LandAnimal landAnimal = new LandAnimal();
landAnimal.AnimalInfo("Dog", "be in door animal.");
landAnimal.AnimalRunSpeed("Horse", 13.3m, "meter");

WaterAnimal waterAnimal = new WaterAnimal();
waterAnimal.AnimalSwimSpeed("Doplin", 2, "meter");

Loop loop1 = new Loop();
Console.WriteLine(loop1.Sum(3, 5, 2));
Console.WriteLine(loop1.ValidateNumber("2", "2"));