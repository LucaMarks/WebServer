#include "HTTPServer.h"

#include <io.h>

#include "Route.h"
#include <stdarg.h>
#include <string.h>
#include "../Systems/ThreadPool.h"

HTTPServer httpServerConstructor();
void registerRoutes(struct HTTPServer *server, void (*routeFunction)(struct HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...);

//since this is a prototype, function names are not necessary
void launch(HTTPServer *);

void *handler(void *arg);

//mimic a private access modifier
typedef struct ClientServer {
    int client;
    HTTPServer *server;
}ClientServer;

HTTPServer httpServerConstructor() {
    HTTPServer server;
    server.server = serverConstructor(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 255);
    server.routes = dictionary_constructor(compare_string_keys);
    server.registerRoutes = registerRoutes;
    return server;
}


void registerRoutes(struct HTTPServer *server, void (*routeFunction)(struct HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...) {
    Route route;
    va_list methods;
    va_start(methods, numMethods);
    for (int i = 0; i < numMethods; i++){
        route.methods[i] = va_arg(methods, int);
    }
    strcpy(route.uri, uri);
    route.routeFunction = &routeFunction;

    (*server).routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));

}

void launch(HTTPServer *server) {
    ThreadPool threadPool = threadPoolConstructor(20);
    struct sockaddr *sockaddr = (struct sockaddr*)&server->server.address;
    unsigned long addrLen = sizeof(server->server.address);
    while (1) {
        ClientServer *clientServer = malloc(sizeof(ClientServer));
        (*clientServer).client = accept(server->server.socket, sockaddr, (socklen_t*)addrLen);
        (*clientServer).server = server;
        ThreadJob job = threadJobConstructor(handler, clientServer);
        threadPool.addWork(&threadPool, &job);
    }

}



void *handler(void *arg) {
    /*if we make clientServer a pointer than we have to free() it manually
     if we don't make it a pointer (as it is in the curr state), then this memory get's freed at the end of the function
     this is a copy of the original clientServer object because it has been dereferenced*/
    // ClientServer clientServer = *(ClientServer*)arg;
    ClientServer *clientServer = (ClientServer*)arg;

    //this is freaky ngl. I 'C' y C is unsafe
    free(arg);

    char requestString[30000];
    read(clientServer->client, &requestString, sizeof(requestString));
    HTTPRequest request = httpRequestConstructor(requestString);
    char *uri = request.requestLine.search(&request.requestLine, "uri", sizeof("uri"));
    Route *route = clientServer->server->routes.search(&clientServer->server->routes, uri, sizeof(char[strlen(uri)]));

    char *resp = route->routeFunction(clientServer->server, &request);
    write(clientServer->client, resp, sizeof(char[strlen(resp)]));
    free(arg);//could also write free(clientServer) -> they have the same functionality
    return NULL;
}












