namespace Homework.Views.Interfaces { 
    public interface IValidationView {
        string GetInput();
        void SetInput(string text);
        void SetResultText(string text);
        void ShowInfo(string message, string caption);
        void ShowWarning(string message, string caption);
        void FocusAndSelectAll();
    }
}