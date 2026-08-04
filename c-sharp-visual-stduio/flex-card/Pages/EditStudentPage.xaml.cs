using Microsoft.Maui.Platform.Linux.Hosting;

namespace user_info.Pages;

public partial class EditStudentPage : ContentPage
{
	public EditStudentPage() {
		InitializeComponent();
	}

    private void GoBack(object sender, EventArgs e) {
        LinuxViewRenderer.PopPage();
    }

    private void GoHome(object sender, EventArgs e) {
        LinuxViewRenderer.NavigateToRoute("HomePage");
    }
}