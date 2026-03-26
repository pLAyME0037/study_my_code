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
import java.time.Duration;

class Mp4Dnloder {
    public static void main(String[] args) {
        Downloader("https://youtu.be/aKnl8-qifvE?si=xnlRag6Id3PeNVTg");
    }

    public static void Mp4Downloader(String link) {
        try {
            URL url = new URL(link);
            URLConnection conn = url.openConnection();

            try (InputStream in = conn.getInputStream();
                 FileOutputStream out = new FileOutputStream("./files/vid_1.mp4")) {

                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            System.out.println("done");
        }
    }

    public static void Downloader(String link) {
        try {
            Path path = Paths.get("./files");
            Files.createDirectories(path);

            ProcessBuilder pb = new ProcessBuilder(
                "./yt-dlp",
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
