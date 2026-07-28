using staff_info.ViewModels;

namespace staff_info;

public partial class MainPage : ContentPage
{
	public MainPage() {
		InitializeComponent();
        BindingContext = new Staff();
	}

}
