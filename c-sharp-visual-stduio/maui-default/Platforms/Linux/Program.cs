using Microsoft.Maui.Platform.Linux;
using Microsoft.Maui.Platform.Linux.Hosting;

namespace maui_default;

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

// MainPage.xaml.cs:1:namespace student_data;
// Platforms/Windows/App.xaml.cs:3:namespace student_data.WinUI;
// Platforms/Windows/App.xaml:2:    x:Class="student_data.WinUI.App"
// Platforms/Windows/App.xaml:6:    xmlns:local="using:student_data.WinUI">
// Platforms/Windows/Package.appxmanifest:8:  <Identity Name="com.companyname.student_data" Publisher="CN=User" Version="1.0.0.0" />
// Platforms/Linux/Program.cs:4:namespace student_data;
// Platforms/Android/MainApplication.cs:4:namespace student_data;
// Platforms/Android/MainActivity.cs:5:namespace student_data;
// App.xaml.cs:1:namespace student_data;
// grep: obj/Debug/net10.0/ref/student-data.dll: binary file matches
// grep: obj/Debug/net10.0/refint/student-data.dll: binary file matches
// grep: obj/Debug/net10.0/student-data.dll: binary file matches
// obj/Debug/net10.0/student-data.GeneratedMSBuildEditorConfig.editorconfig:19:build_property.RootNamespace = student_data
// obj/Debug/net10.0/student-data.GeneratedMSBuildEditorConfig.editorconfig:28:build_metadata.AdditionalFiles.ManifestResourceName = student_data.App.xaml
// obj/Debug/net10.0/student-data.GeneratedMSBuildEditorConfig.editorconfig:38:build_metadata.AdditionalFiles.ManifestResourceName = student_data.MainPage.xaml
// obj/Debug/net10.0/student-data.GeneratedMSBuildEditorConfig.editorconfig:48:build_metadata.AdditionalFiles.ManifestResourceName = student_data.Resources.Styles.Colors.xaml
// obj/Debug/net10.0/student-data.GeneratedMSBuildEditorConfig.editorconfig:58:build_metadata.AdditionalFiles.ManifestResourceName = student_data.Resources.Styles.Styles.xaml
// App.xaml:4:             x:Class="student_data.App">
// student-data.csproj:14:    <RootNamespace>student_data</RootNamespace>
// student-data.csproj:18:    <ApplicationId>com.companyname.student_data</ApplicationId>
// grep: bin/Debug/net10.0/student-data.dll: binary file matches
// MainPage.xaml:4:             x:Class="student_data.MainPage"
// MauiProgram.cs:4:namespace student_data;
