using user_info.ViewModels;

namespace user_info.Views;

public partial class Staff : ContentPage
{
	public Staff() {
		InitializeComponent();

        BindingContext = new StaffVM();
	}
}
