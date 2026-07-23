using StudentInformationApp.ViewModels;
namespace StudentInformationApp;

public partial class MainPage : ContentPage
{
    public MainPage()
    {
        InitializeComponent();
        BindingContext = new StudentViewModel();
    }
}
