namespace ConsoleApp1.Day2
{
    class Student
    {
        public int studentId { get; set; } = 0;
        public string name { get; set; } = "N/A";
        public string sex { get; set; } = "N/A";

        public string SetSudentInfo()
        {
            return $"info: {studentId}, {name}, {sex}";
        }

        public override string ToString()
        {
            return $"info: {studentId}, {name}, {sex}";
        }
        public string studentAdd(string name)
        {
            return "Successfully Add a student at " + name;
        }

        public string studentDelete(int studentId)
        {
            return "Successfully Deleted a student at " + studentId;
        }
    }
}
