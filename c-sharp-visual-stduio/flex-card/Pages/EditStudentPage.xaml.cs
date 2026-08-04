using user_info.Services;

namespace user_info.Pages;

public partial class EditStudentPage : ContentPage
{
	public EditStudentPage() {
		InitializeComponent();
	}

    private void GoBack(object sender, EventArgs e) {
        Nav.Pop();
    }

    private void GoHome(object sender, EventArgs e) {
        Nav.GoHome();
    }
}