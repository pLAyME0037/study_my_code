using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace StudentInformationApp.ViewModels;

public partial class StudentViewModel : ObservableObject
{
    [ObservableProperty]
    private string studentId = "";

    [ObservableProperty]
    private string name = "";

    [ObservableProperty]
    private string major = "";

    [ObservableProperty]
    private string gpa =" 0.0";

    [RelayCommand]
    private void LoadStudent()
    {
        StudentId = "TK-0011";
        Name = "Suon Tongkoeng";
        Major = "Computer Science";
        Gpa = "3.6";
    }

    [RelayCommand]
    private void Clear()
    {
        StudentId = "";
        Name = "";
        Major = "";
        Gpa ="0.0";
    }

}
