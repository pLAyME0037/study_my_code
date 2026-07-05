using System.Collections.Generic;
using System.Windows.Forms;

namespace Homework.Helpers
{
    // Simple, reusable navigation utility that hides the "from" form and shows the "to" form.
    // Call Navigate(from, to) for modeless navigation or NavigateModal(from, to) for modal dialogs.
    public static class NavigationManager
    {
        private static readonly Stack<Form> _stack = new Stack<Form>();

        public static void Navigate(Form from, Form to)
        {
            if (from == null || to == null) return;
            _stack.Push(from);
            from.Hide();

            // When the target closes, show the previous form (if any).
            to.FormClosed += (_, __) =>
            {
                if (_stack.Count > 0)
                {
                    var prev = _stack.Pop();
                    prev.Show();
                }
            };

            to.Show();
        }

        public static void NavigateModal(Form from, Form to)
        {
            if (from == null || to == null) return;
            _stack.Push(from);
            from.Hide();
            to.ShowDialog();
            // After modal returns, show previous
            if (_stack.Count > 0)
            {
                var prev = _stack.Pop();
                prev.Show();
            }
        }

        // Closes the current form and returns to the previous.
        public static void NavigateBack(Form current)
        {
            current.Close(); // FormClosed handler in Navigate will reveal previous form.
        }
    }
}