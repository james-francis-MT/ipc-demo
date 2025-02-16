#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    int result;

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket5.sock");

    int slen = sizeof(server_addr);

    unlink(server_addr.sun_path);

    bind(server_socket, (struct sockaddr *) &server_addr, slen);

    listen(server_socket, 5);

    while(1){
        char ch[200];
        int clen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &clen);
        read(client_socket, &ch, 200);
        ch[199] = '\0';
        printf("\nServer: I recieved %s from client!\n", ch);
        close(client_socket);
    }

    exit(0);
}
