#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

const char *buffer[4096];

int main(int argc, char **argv) {
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	inet_pton(AF_INET, argc > 1 ? argv[1] : "127.0.0.1", &local.sin_addr);
	local.sin_port = ntohs(9090);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock, (void*)&local, sizeof(local));
    listen(sock, 123);

    int client = accept(sock, NULL, NULL);
    write(client, "220 Welcome\r\n", 13);

    int bytesRead = 0, res;
    for (;;) {
        res = read(client, buffer, sizeof(buffer));
        if (res < 0)  {
            perror("read");
            // exit(1);
            break;
        }
        if (!res) break;
        bytesRead += res;
    }
    printf("%d\n", bytesRead);

    return 0;
}
