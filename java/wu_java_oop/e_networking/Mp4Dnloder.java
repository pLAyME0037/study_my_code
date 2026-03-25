import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URLConnection;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.Duration;

class Mp4Dnloder {
    public static void main(String[] args) throws Exception {
        // Downloader("https://youtu.be/kS_GqDp6IT4");
        modernDownloader("https://youtu.be/aKnl8-qifvE?si=xnlRag6Id3PeNVTg");
    }

    public static void Mp4Downloader(String spec) {
        try {
            URL url = new URL(spec);
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

    public static void Downloader(String spec) {
        try {
            Path path = Paths.get("./files");
            Files.createDirectory(path);

            URL url = new URI(spec).toURL();
            URLConnection conn = url.openConnection();

            conn.setRequestProperty("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");

            try (InputStream in = conn.getInputStream();
                 FileOutputStream out = new FileOutputStream(path.resolve("./files/vid_1.mp4").toFile())) {

                byte[] buffer = new byte[8192];
                int bytesRead;
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                }
                System.out.println("Download successful!");
            }
        } catch (URISyntaxException e) {
            System.err.println("URI Error: " + e.getMessage());
        } catch (IOException e) {
            System.err.println("IO Error: " + e.getMessage());
        }
    }

    public static void modernDownloader(String link) {
        try {
            HttpClient client = HttpClient.newBuilder()
                .followRedirects(HttpClient.Redirect.NORMAL)
                .connectTimeout(Duration.ofSeconds(10))
                .build();

            HttpRequest request = HttpRequest.newBuilder()
                .uri(URI.create(link))
                .header("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")
                .GET()
                .build();

            Path outputPath = Paths.get("./files/video.mp4");
            Files.createDirectories(outputPath.getParent());

            HttpResponse<Path> response = client.send(request, HttpResponse.BodyHandlers.ofFile(outputPath));

            if (response.statusCode() == 200) {
                System.out.println("Success! File saved to: " + response.body().toAbsolutePath());
            } else {
                System.err.println("Failed! Server returned: " + response.statusCode());
                Files.deleteIfExists(outputPath);
            }

        } catch (Exception e) {
            System.err.println("Error during download: " + e.getMessage());
        }
    }
}
