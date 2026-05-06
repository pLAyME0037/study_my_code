import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URLConnection;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.time.Duration;

class Mp4Dnloder {
    public static void main(String[] args) {
        docDownloader("https://flgr.sh/txtfssAWWtxt");
        // docDownloader("https://raw.githubusercontent.com/pLAyME0037/APPDATAor.dotfile/refs/heads/main/bashrc/bashrc.sh");
        // vidDownloader("https://youtu.be/sVPYIRF9RCQ?si=3otIpNw99xX1wTQW");
    }

    public static void docDownloader(String link) {
        try {
            URL url = new URL(link);

            Path targetPath = Path.of("./files/doc.txt");

            Files.createDirectories(targetPath.getParent());

            InputStream in = url.openStream();
            Files.copy(in, targetPath, StandardCopyOption.REPLACE_EXISTING);

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            System.out.println("done");
        }
    }

    public static void vidDownloader(String link) {
        try {
            Path path = Paths.get("./files");
            Files.createDirectories(path);

            ProcessBuilder pb = new ProcessBuilder(
                "yt-dlp",
                "-o", path.resolve("%(title)s.%(ext)s").toString(),
                link
            );
            pb.inheritIO();
            Process process = pb.start();
            int exitCode = process.waitFor();

            if (exitCode == 0) {
                System.out.println("Download successful!");
            } else {
                System.err.println("Download failed with exit code: " + exitCode);
            }
        } catch (IOException | InterruptedException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
