#include <stdio.h>
#include "Server.h"
#include <unistd.h>

void launch(struct Server *server){
    printf("========= WAITING FOR CONNECTION ==========\n");

    char buffer[3000];
    int addressLength = sizeof(server->address);
    int new_socket;
    char *hello =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 47\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Hello World!</h1></body></html>";
    while(1) {

        new_socket = accept((*server).socket, (struct sockaddr*)&server->address, (socklen_t*)&addressLength);

        recv(new_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);

        send(new_socket, hello, strlen(hello), 0);
        closesocket(new_socket);
    }
}

int main(){
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;
    }

    struct Server server = serverConstructor(
        AF_INET, SOCK_STREAM, IPPROTO_TCP, 80, INADDR_ANY, 10, launch
    );

    server.launch(&server);
    WSACleanup();
}

