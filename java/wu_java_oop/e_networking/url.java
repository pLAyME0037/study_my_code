import java.net.URL;

class url {
    public static void main(String[] args) {
        URL url = new URL("https://search.brave.com/images?q=wallpaper");

        System.out.println("URL is:" + url.toString());
    }
}
