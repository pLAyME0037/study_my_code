using user_info.Services;

namespace user_info.Pages;

public partial class HomePage : ContentPage
{
	public HomePage() {
		InitializeComponent();
	}

    private void StudentListClicked(object sender, EventArgs e) {
        Nav.Push(new StudentList());
    }
}
