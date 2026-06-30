#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <asm-generic/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const char *buffer[1000000];

int main(int argc, char **argv) {
    memset(buffer, 'a', sizeof(buffer));

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct linger so_linger;
    so_linger.l_onoff = true;
    so_linger.l_linger = 30;
    setsockopt(sock, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	inet_pton(AF_INET, argc > 1 ? argv[1] : "127.0.0.1", &remote.sin_addr);
	remote.sin_port = ntohs(9090);

    connect(sock, (void*)&remote, sizeof(remote));
    write(sock, buffer, 1000000);             // returns 1000000
    shutdown(sock, SHUT_WR);
    for (;;) {
        int res = read(sock, buffer, 4000);
        if (res < 0) {
            perror("reading");
            exit(1);
        }
        if (!res) break;
    }
    close(sock);

    return 0;
}
