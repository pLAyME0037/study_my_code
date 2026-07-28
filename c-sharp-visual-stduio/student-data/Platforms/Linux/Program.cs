using Microsoft.Maui.Platform.Linux;
using Microsoft.Maui.Platform.Linux.Hosting;

namespace student_data;

public class Program
{
    public static void Main(string[] args)
    {
        var app = MauiProgram.CreateMauiAppBuilder()
            .UseLinux()
            .Build();
        LinuxApplication.Run(app, args);
    }
}
