using user_info.Services.StudentDB;

namespace user_info.Views.StudentDB;

public partial class StudentList : ContentPage
{
    private readonly DBService _dbService;

    public StudentList() {
        InitializeComponent();
        _dbService = new DBService();
    }

    protected override async void OnAppearing() {
        base.OnAppearing();
        try {
            await _dbService.InitializeDatabaseAsync();
            var students = await _dbService.GetStudentAsync();
            StudentCollectionView.ItemsSource = students;
        } catch (Exception ex) {
            await DisplayAlertAsync("Error: ", ex.Message, "OK");
        }
    }

    protected async void OnSearchClicked(object sender, EventArgs e) {
        try {
            string searchName = SearchEntry.Text?.Trim() ?? "";
            if (string.IsNullOrWhiteSpace(searchName)) {
                var students = await _dbService.GetStudentAsync();
                StudentCollectionView.ItemsSource = students;
                return;
            }
            var results = await _dbService.SearchStudentsByNameAsync(searchName);
            StudentCollectionView.ItemsSource = results;
        } catch (Exception ex) {
            await DisplayAlertAsync("Error: ", ex.Message, "OK");
        }
    }
}


