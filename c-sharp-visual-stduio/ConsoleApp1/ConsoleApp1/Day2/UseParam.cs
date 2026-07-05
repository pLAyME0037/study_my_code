namespace ConsoleApp1.Day2
{
    class UseParam
    {
        public int Sum(params int[] nums)
        {
            int total = 0;
            foreach (int i in nums)
            {
                total += i;
            }
            return total;
        }
        public string ConcatString(params string[] words)
        {
            string concated = "";
            for (int i = 0; i != words.Length; i++)
            {
                concated += words[i];
            }
            return concated;
        }

        public string ConcatStrUseForeach(params string[] words)
        {
            string result = "";

            foreach (string word in words)
            {
                result += word;
            }
            return result;
        }
    }
}
