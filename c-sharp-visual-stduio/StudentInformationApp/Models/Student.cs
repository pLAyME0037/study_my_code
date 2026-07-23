using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Text;

namespace StudentInformationApp.Models
{
    public class Student {
        [Required]
        public string studentId { get; set; } = "";
        [Required]
        public string name { get; set; } = "";
        [Required]
        public string major {  get; set; } = "";
        [Required]
        public string gpa { get; set; } = "";
    }
}
