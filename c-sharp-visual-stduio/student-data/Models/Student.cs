using System.ComponentModel.DataAnnotations;

namespace student_data.Models;
public class Student {
    [Required]
    public string StudentId { set; get; } = "";
    [Required]
    public string Name { set; get; } = "";
    [Required]
    public int Age { set; get; }
    [Required]
    public string Major { set; get; } = "";

}
