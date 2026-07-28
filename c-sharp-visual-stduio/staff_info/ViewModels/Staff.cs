using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace staff_info.ViewModels;

public partial class Staff : ObservableObject
{
    [ObservableProperty]
    private string staffId    = "";
    [ObservableProperty]
    private string name       = "";
    [ObservableProperty]
    private float salary     = 0.0f;
    [ObservableProperty]
    private string department = "";

    [RelayCommand]
    private void loadStaff() {
        StaffId    = "01011";
        Name       = "dara";
        Salary     = 999.99f;
        Department = "Art";
    }

    [RelayCommand]
    private void clearStaff() {
        StaffId    = "";
        Name       = "";
        Salary     = 0.0f;
        Department = "";
    }

    [RelayCommand]
    private void increaseStaffSalary() {
        Salary += 100.0f;
    }
}
