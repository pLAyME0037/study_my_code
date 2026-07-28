using Microsoft.Maui.Controls.Hosting;
using Microsoft.Maui.Hosting;

namespace student_data;

public static class MauiProgram {
    public static MauiAppBuilder CreateMauiAppBuilder() {
        var builder = MauiApp.CreateBuilder();

        builder
            .UseMauiApp<App>()
            .ConfigureFonts(fonts => {
                fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
            });

        return builder;
    }

    public static MauiApp CreateMauiApp() {
        return CreateMauiAppBuilder().Build();
    }
}
