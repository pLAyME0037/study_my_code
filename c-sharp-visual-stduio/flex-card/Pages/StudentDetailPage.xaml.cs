using user_info.Services;

namespace user_info.Pages;

public partial class StudentDetailPage : ContentPage
{
	public StudentDetailPage() {
		InitializeComponent();
	}

    private void EditStudent(object sender, EventArgs e) {
        Nav.Push(new EditStudentPage());
    }
}