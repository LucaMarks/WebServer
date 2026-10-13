#include <stdio.h>
#include "Networking/Nodes/HTTPServer.h"

// int main(void) {
    // printf("Hello, World!\n");
    // return 0;
// }

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return 1;    }

    struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 20, launch);
    server.launch(&server);

    WSACleanup();
}