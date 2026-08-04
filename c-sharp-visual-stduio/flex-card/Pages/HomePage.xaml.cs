using Microsoft.Maui.Platform.Linux.Hosting;

namespace user_info.Pages;

public partial class HomePage : ContentPage 
{
	public HomePage() {
		InitializeComponent();
	}

    private void StudentListClicked(object sender, EventArgs e) {
        LinuxViewRenderer.PushPage(new StudentList());
    }
}