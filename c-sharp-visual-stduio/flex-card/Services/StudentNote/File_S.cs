using System;
using System.Text.Json;
using user_info.Models.StudentNote;

namespace user_info.Services.StudentNote;

public class File_S
{
    private readonly string _filePath;

    public File_S() {
        string baseDir;
        try {
            baseDir = FileSystem.Current.AppDataDirectory;
        } catch (Exception) {
            baseDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
        }
        _filePath = Path.Combine(baseDir, "student_note.json");
    }

    public async Task SaveAsync(Note note) {
        string json = JsonSerializer.Serialize(note);
        await File.WriteAllTextAsync(_filePath, json);
    }

    public async Task<Note?> LoadAsync() {
        string json = await File.ReadAllTextAsync(_filePath);
        return JsonSerializer.Deserialize<Note>(json);
    }

    public void Delete() {
        File.Delete(_filePath);
    }
}

