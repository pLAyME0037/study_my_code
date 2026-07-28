using student_data.Models;

namespace student_data;

public partial class MainPage : ContentPage
{
    private int _count = 0;

    public MainPage() {
        InitializeComponent();

        // Wire up slider value changed
        VolumeSlider.ValueChanged += OnVolumeChanged;
    }

    private void OnCounterClicked(object sender, EventArgs e) {
        _count++;

        CounterBtn.Text = _count == 1
            ? $"Clicked {_count} time"
            : $"Clicked {_count} times";

        SemanticScreenReader.Announce(CounterBtn.Text);
    }

    private void OnVolumeChanged(object? sender, ValueChangedEventArgs e) {
        VolumeLabel.Text = $"Volume: {e.NewValue:F0}";
    }

    private void OnFillStud(object sender, EventArgs e) {
        Student stu = new() {
            StudentId = "0001",
            Name = "So idk",
            Age = 32,
            Major = "Computer Science",
        };

        IdEntry.Text    = stu.StudentId;
        NameEntry.Text  = stu.Name;
        AgeEntry.Text   = stu.Age.ToString();
        MajorEntry.Text = stu.Major;
    }

    private void OnClear(object sender, EventArgs e) {
        IdEntry.Text    = string.Empty;
        NameEntry.Text  = string.Empty;
        AgeEntry.Text   = string.Empty;
        MajorEntry.Text = string.Empty;
    }
}
