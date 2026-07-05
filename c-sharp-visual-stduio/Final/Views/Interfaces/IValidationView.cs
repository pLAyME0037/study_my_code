using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Final.Views.Interfaces
{
    public interface IValidationView
    {
        string GetInput();
        void SetInput(string text);
        void SetResultText(string text);
        void ShowInfo(string message, string caption);
        void ShowWarning(string message, string caption);
        void FocusAndSelectAll();
    }
}
