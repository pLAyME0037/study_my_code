using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsApp1.Model
{
    public class StudentModel
    {
        private string text1;
        private string text2;

        public int id { get; set; } = 0;
        public string name { get; set; } = "N/A";
        public string sex { get; set; } = "N/A";
        //public StudentModel() {
        //    return "";
        //}

        public StudentModel(string text1, string text2)
        {
            this.text1 = text1;
            this.text2 = text2;
        }

        public StudentModel()
        {
            id = 0;
            name = "";
            sex = "";
        }

        public string orgAtt()
        {
            return $"is: {id} {name} {sex}";
        }

        public string studentAdd(string name, string sex)
        {
            return "successfully Added a student: Name: " 
                + name + " sex: " 
                + sex;
        }

        public string studentDelete(int id)
        {
            return "Successfully Delete a student at " + id;
        }

        public StudentModel studentInfo()
        {
            StudentModel studentMd = new StudentModel
            {
                id = 1,
                name = "dara",
                sex = "M"
            };
            return studentMd;
        }

        public List<StudentModel> studentsList()
        {
            List<StudentModel> studentMdL = new List<StudentModel>
            {
                new StudentModel { id = 1, name = "sok", sex = "M"},
                new StudentModel { id = 2, name = "fin", sex = "M"},
                new StudentModel { id = 3, name = "gill", sex = "F"},
            };
            return studentMdL;
        }
    }
}
