using System.ComponentModel.DataAnnotations;

namespace user_info.Models;

public class Staff {
    [Required]
    public string staffId    { get; set; } = "";
    [Required]
    public string name       { get; set; } = "";
    [Required]
    public string salary     { get; set; } = "";
    [Required]
    public string department { get; set; } = "";
}
