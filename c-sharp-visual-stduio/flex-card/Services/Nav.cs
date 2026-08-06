using Microsoft.Maui.Controls;

namespace user_info.Services;

public static class Nav
{
    public static void Push(Page page) {
#if ANDROID || IOS || MACCATALYST || WINDOWS
        Shell.Current.GoToAsync(page.GetType().Name);
#else
        Microsoft.Maui.Platform.Linux.Hosting.LinuxViewRenderer.PushPage(page);
#endif
    }

    public static void Pop() {
#if ANDROID || IOS || MACCATALYST || WINDOWS
        Shell.Current.Navigation.PopAsync();
#else
        Microsoft.Maui.Platform.Linux.Hosting.LinuxViewRenderer.PopPage();
#endif
    }

    public static void GoHome() {
#if ANDROID || IOS || MACCATALYST || WINDOWS
        Shell.Current.GoToAsync("//HomePage");
#else
        Microsoft.Maui.Platform.Linux.Hosting.LinuxViewRenderer.CurrentSkiaShell?.NavigateToSection(0);
#endif
    }
}
