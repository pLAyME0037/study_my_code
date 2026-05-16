# Package java.net
package **java.net** Provides the classes for implementing networking applications.
The java.net package can be roughly divided in two sections:

  - A Low Level API, which deals with the following abstractions:
    - Addresses, which are networking identifiers, like IP addresses.
    - Sockets, which are basic bidirectional data communication mechanisms.
    - Interfaces, which describe network interfaces.

  - A High Level API, which deals with the following abstractions:
    - URIs, which represent Universal Resource Identifiers.
    - URLs, which represent Universal Resource Locators.
    - Connections, which represents connections to the resource pointed to by URLs.

### Advantages of Java Networking
- Creating server-client applications
- Implementing networking protocols
- Implement socket programming
- Creating web services

**Socket Programming**
  Sockets are means to establish a communication link between machines over the
  network. 
  The java.net package provides 4 kinds of Sockets:

  - ***Socket*** is a TCP client API, and will typically be used to connect to a
    remote host.

  - ***ServerSocket*** is a TCP server API, and will typically accept connections
    from client sockets.

  - ***DatagramSocket*** is a UDP endpoint API and is used to send and receive
    datagram packets.

  - ***MulticastSocket*** is a subclass of DatagramSocket used when dealing with
    multicast groups.
  
  Sending and receiving with TCP sockets is done through InputStreams and
  OutputStreams which can be obtained via the Socket.getInputStream() and
  Socket.getOutputStream() methods.

### Socket Programming in Java Networking
* Sockets provide the communication mechanism between two computers using TCP. 
  A client program creates a socket on its end of the communication and attempts 
  to connect that socket to a server.

  1. The "Hidden" Door Number (Ephemeral Port)
  When you (the client) create a socket, you usually don't pick your own door 
  number. The operating system looks at its list of available doors and says: "
  You can use Door #52849 for this conversation."

    * The Server's Door: Is usually famous and fixed (like Port 80 for Web or 
      Port 22 for SSH).
    * The Client's Door: Is temporary and random (called an ephemeral port).

  2. The Handshake
  When the client "attempts to connect," it sends a message to the server that 
  says: **"Hey Server, I’m knocking on your Door 80. If you want to talk back to 
  me, send your replies to my IP Address at Door 52849."**
  
  3. Creating the "Virtual Pipe"
  Once the server accepts, the two "ends" are linked. Even though there might be 
  thousands of people talking to that same server, the server knows that:
  
    * Socket A = Conversation with You (via your unique door number).
    * Socket B = Conversation with Someone Else (via their unique door number).
  
* When the connection is made, the server creates a socket object on its end of
  the communication. The client and the server can now communicate by writing to
  and reading from the socket.

* The java.net.Socket class represents a socket, and the java.net.ServerSocket
  class provides a mechanism for the server program to listen for clients and
  establish connections with them.
```java
public static class socket_client {
    public static void main(String[] args) {
        String serverName = args[0];
        int port = Integer.parseInt(args[1]);

        try (Socket client = new Socket(serverName, port);
                DataOutputStream out = new DataOutputStream(client.getOutputStream());
                DataInputStream in = new DataInputStream(client.getInputStream())) {

            System.out.format("[Info] Connecting to %s on port %d %n",
                    serverName, port);
            System.out.println("[Info] Connected to" + client.getRemoteSocketAddress());

            out.writeUTF("Hello from" + client.getLocalSocketAddress());

            System.out.println("[Info] From Server: " + in.readUTF());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

**URL Processing**
URL stands for **Uniform Resource Locator** and represents a resource on the World 
Wide Web, such as a Web page or FTP directory.

A URL can be broken down into parts, as follows (default port 80) −
```console
protocol://host:port     /path     ?query      #ref
https   ://www.amrood.com/index.htm?language=en#j2se
```

