namespace ConsoleApp1.Day2
{
    class IfUsage
    {
        public string ConvertToDay(int dayNum)
        {
            string result = "N/A";

            if (dayNum == 1)
            {
                result = "ចន្ទ័ \" + \"Monday";
            }
            else if (dayNum == 2)
            {
                result = "អង្គារ  \" + \"Tuesday";
            }
            else if (dayNum == 3)
            {
                result = "ពុធ  \" + \"Wednesday";
            }
            else if (dayNum == 4)
            {
                result = "ព្រហស្សត៏  \" + \"Thursday";
            }
            else if (dayNum == 5)
            {
                result = "សុក្រ  \" + \"Friday";
            }
            else if (dayNum == 6)
            {
                result = "សៅរ៏  \" + \"Saterday";
            }
            else if (dayNum == 7)
            {
                result = "អាទិត្យ  \" + \"Sunday";
            }

            return result;
        }
    }
}
