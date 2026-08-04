using Microsoft.Maui.Platform.Linux.Hosting;

namespace user_info.Pages;

public partial class StudentDetailPage : ContentPage
{
	public StudentDetailPage() {
		InitializeComponent();
	}

    private void EditStudent(object sender, EventArgs e) {
        LinuxViewRenderer.PushPage(new EditStudentPage());
    }
}