import java.net.URL;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URI;

class Networking {
    public static void main(String[] args) {
        // createUrl();
        int port;

        if (args.length < 1) {
            System.out.println("Usage: Java networking <8080>");
            port = 8080;
        } else {
            port = Integer.parseInt(args[0]);
        }

        try {
            socket_server servThread = new socket_server(port);
            servThread.start();
            Thread.sleep(500);

            if (port == 8080) socket_client.main(new String[] { "localhost", "8080" });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void createUrl() {
        try {
            URI baseUri = new URI("http://sample.com/pages/");

            URL home_url = baseUri.resolve("home.html").toURL();
            URL aboutme_url = baseUri.resolve("aboutme.html").toURL();

            System.out.println(home_url);
            System.out.println(aboutme_url);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static class socket_client {
        public static void main(String[] args) {
            String serverName = args[0];
            int port = Integer.parseInt(args[1]);

            try {
                Socket client = new Socket(serverName, port);
                System.out.format("[Info] Connecting to %s on port %d %n",
                                  serverName, port);
                System.out.println("[Info] Connected to" + client.getRemoteSocketAddress());

                // OutputStream toServer = client.getOutputStream();
                DataOutputStream out = new DataOutputStream(client.getOutputStream());
                out.writeUTF("Hello from" + client.getLocalSocketAddress());

                // InputStream fromServer = client.getInputStream();
                DataInputStream in = new DataInputStream(client.getInputStream());
                System.out.println("[Info] From Server: " + in.readUTF());

                client.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static class socket_server extends Thread {
        private ServerSocket serverSocket;

        public socket_server(int port) throws IOException {
            serverSocket = new ServerSocket(port);
            serverSocket.setSoTimeout(0);
        }

        public static void main(String[] args) {
            int port = Integer.parseInt(args[0]);
            try {
                Thread t = new socket_server(port);
                t.start();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            while (true) {
                try {
                    System.out.println("[Info] Waiting for client on port "
                                      + serverSocket.getLocalPort()
                                      + "...");
                    Socket server = serverSocket.accept();
                    System.out.println("[Info] Connected to "
                                      + server.getRemoteSocketAddress());

                    DataInputStream in = new DataInputStream(server.getInputStream());
                    System.out.println(in.readUTF());

                    DataOutputStream out = new DataOutputStream(server.getOutputStream());
                    out.writeUTF("Welcome you are now connected to"
                                + server.getLocalSocketAddress()
                                + "\nGoodbye!");

                    // server.close();
                } catch (IOException e) {
                    e.printStackTrace();
                    break;
                }
            }
        }
    }
}

