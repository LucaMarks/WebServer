#include "HTTPServer.h"
// #include "../../"
#include<stdarg.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>


void launch(Server *server);
void *handler(void *arg);

void registerRoutes(HTTPServer *server, char *(*routeFunction)(HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...);

typedef struct ClientServer {
    int client;
    HTTPServer *server;
}ClientServer;

//route struct is stored in HTTPServer.routes dict
typedef struct Route{
    int methods[9];
    char *uri;
    char *(*routeFunction)(HTTPServer *server, HTTPRequest *request);
}Route;


// HTTPServer httpServerConstructor() {
//     HTTPServer server;
//     server.server = serverConstructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 255, launch);
//     server.routes = dictionary_constructor(compare_string_keys);
//     server.registerRoutes = registerRoutes;
//     server.launch = launch;
//     return server;
//
// }


// void registerRoutes(HTTPServer *server, char *(*routeFunction)(HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...) {
//     struct Route route;
//
//     //comes from stdarg.h
//     va_list methods;
//     va_start(methods, numMethods);
//     for (int i = 0; i < numMethods; i++) {
//         route.methods[i] = va_arg(methods, int);
//     }
//     //register the uri
//     char buff[strlen(uri)];
//     route.uri = buff;
//     strcpy(route.uri, uri);
//     //copy the desired uri
//     route.routeFunction = routeFunction;
//     //save the route in server's dir
//     server->routes.insert(&(server->routes), uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
// }

//prototype for selecting a page to show the user
void retrieve_page(HTTPRequest request, int socket) {
    char path[30000] = {0};

    char *url = strtok(request.requestLine.search(&request.requestLine, "uri", sizeof("uri")), "?");
    char *vars = strtok(NULL, "\0");

    strcpy(path, "/Users/lucst/documents/projects");
    if (strcmp(url, "/test") == 0) {strcat(path, url);}
    else{strcat(path, "/index");}

    strcat(path, ".html");

    FILE *f = fopen(path, "r");
    fseek(f, 0, SEEK_END);
    long fSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buff = malloc(fSize +1);
    fread(buff, 1, fSize, f);
    fclose(f);

    // printf("Made it here\n");
    char response[30000] = {0};

    strcpy(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
    strcat(response, buff);

    send(socket, response, strlen(response), 0);
    free(buff);
}

//try this for now
void launch(Server *server) {
    int addrlen = sizeof(server->address);
    long valread;
    while (1) {
        printf("===Waiting===\n");

        int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&addrlen);
        char buff[30000];
        valread = recv(new_socket, buff, sizeof(buff) -1, 0);
        if (valread <= 0) {
            if (valread == SOCKET_ERROR) {fprintf(stderr, "recv failed: %d\n", WSAGetLastError());}
            closesocket(new_socket);
            continue;
        }
        buff[valread] = '\0';
        HTTPRequest request = httpRequestConstructor(buff);
        const char *host = request.headerFields.search(&request.headerFields, "Host", sizeof("Host"));
        const char *body = host ? host : "Host header not found";

        // char response[3000];
        // int len = snprintf(response, sizeof(response),
        //     "HTTP/1.1 200 ok\r\n"
        //     "Content-Type: text/plain\r\n"
        //     "Content-Length: %zu\r\n"
        //     "Connection: close\r\n"
        //     "\r\n"
        //     "%s",
        //     strlen(body), body);
        // if (len < 0 || (size_t)len >= sizeof(response)) {
        //     closesocket(new_socket);
        //     continue;
        // }
        //
        // int total = 0;
        // while (total < len) {
        //    int sent = send(new_socket, response + total, len - total, 0);
        //     if (sent = SOCKET_ERROR) {
        //         fprintf(stderr, "send failed: %d\n", WSAGetLastError());
        //         break;
        //     }
        //     if (sent == 0) {break;}
        //     total += sent;
        // }

        retrieve_page(request, new_socket);
        closesocket(new_socket);
    }
}


// void launch(HTTPServer, *server) {
//     //initialize a thread pool to handle clients
//     struct ThreadPool threadPool = thread_pool
// }



