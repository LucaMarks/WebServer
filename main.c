#include <stdio.h>

// int main(void) {
    // printf("Hello, World!\n");
    // return 0;
// }

// int main() {
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         printf("WSAStartup failed: %d\n", WSAGetLastError());
//         return 1;    }
//
//     struct Server server = serverConstructor(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 20, launch);
//     server.launch(&server);
//
//     WSACleanup();
// }

typedef struct HTTPRequest {
    int x;
}HTTPRequest;
typedef struct HTTPServer {
    int x;
    void (*registerRoutes)(struct HTTPServer *server, char *(*routeFunction)(struct HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...);
}HTTPServer;

char *renderTemplate();

char *example (HTTPServer *server, HTTPRequest *request) {
    return renderTemplate();
}

int main() {
    HTTPServer server;
    server.registerRoutes(&server, example, "/example", 2, 0, 1);
    // server.launch();
}