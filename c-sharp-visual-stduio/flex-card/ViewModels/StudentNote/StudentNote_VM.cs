using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using user_info.Models.StudentNote;
using user_info.Services.StudentNote;

namespace user_info.ViewModels.StudentNote;

public partial class StudentNote_VM : ObservableObject
{
    private readonly File_S _fileService;

    [ObservableProperty]
    private string _title = string.Empty;
    [ObservableProperty]
    private string _content = string.Empty;
    [ObservableProperty]
    private string _status = "Ready";

    public StudentNote_VM() {
        _fileService = new File_S();
    }

    [RelayCommand]
    public async Task Save() {
        var note = new Note {
            Title = Title,
            Content = Content,
        };

        await _fileService.SaveAsync(note);
        Status = "Note Save Successfully";
    }

    [RelayCommand]
    public async Task Load() {
        var note = await _fileService.LoadAsync();
        if (note == null) {
            Status = "No Save Note Found";
            return;
        }
        Title = note.Title;
        Content = note.Content;
        Status = "Note Load Successfully";
    }

    [RelayCommand]
    public void Delete() {
        _fileService.Delete();
        Title = string.Empty;
        Content = string.Empty;
        Status = "Note Deleted";
    }
}

