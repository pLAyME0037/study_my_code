import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;

class Mp4Dnloder {
    public static void main(String[] args) {
        Downloader("https://youtu.be/kS_GqDp6IT4");
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
            // 1. Ensure the directory exists
            java.nio.file.Files.createDirectories(java.nio.file.Paths.get("./files"));

            URL url = new URL(spec);
            URLConnection conn = url.openConnection();

            // 2. Set a User-Agent (Some servers block Java's default "Java/1.x" agent)
            conn.setRequestProperty("User-Agent", "Mozilla/5.0");

            try (InputStream in = conn.getInputStream();
                 FileOutputStream out = new FileOutputStream("./files/vid_1.mp4")) {

                byte[] buffer = new byte[8192];
                int bytesRead;
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                }
                System.out.println("Download successful!");
            } 
        } catch (IOException e) {
            System.err.println("Error: " + e.getMessage());
        }
    }
}
