using user_info.ViewModels.StudentNote;

namespace user_info.Views;

public partial class StudentNote : ContentPage
{
    public StudentNote() {
        InitializeComponent();

        BindingContext = new StudentNote_VM();
    }
}


