using user_info.Services;
using System.ComponentModel;

namespace user_info.Pages;

public partial class StudentList : ContentPage
{
    public StudentList() {
        InitializeComponent();
    }

    private void StudentDetail1(object sender, EventArgs e) {
        Nav.Push(new StudentDetailPage());
    }

    private void StudentDetail2(object sender, EventArgs e) {
        Nav.Push(new StudentDetailPage());
    }
}