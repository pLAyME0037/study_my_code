using user_info.ViewModels;

namespace user_info.Views;

public partial class MainPage : ContentPage
{
    public MainPage() {
        InitializeComponent();

        BindingContext = new MainPageViewModel();
    }
}


