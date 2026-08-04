using Microsoft.Maui.Platform.Linux.Hosting;
using System.ComponentModel;

namespace user_info.Pages;

public partial class StudentList : ContentPage
{
    public StudentList() {
        InitializeComponent();
    }

    private void StudentDetail1(object sender, EventArgs e) {
        LinuxViewRenderer.PushPage(new StudentDetailPage());
    }

    private void StudentDetail2(object sender, EventArgs e) {
        LinuxViewRenderer.PushPage(new StudentDetailPage());
    }
}