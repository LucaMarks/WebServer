#include "HTTPServer.h"
#include "Route.h"
#include <stdarg.h>
#include <string.h>

HTTPServer httpServerConstructor();
void registerRoutes(struct HTTPServer *server, void (*routeFunction)(struct HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...);

//since this is a prototype, function names are not necessary
void launch(HTTPServer *);

HTTPServer httpServerConstructor() {
    HTTPServer server;
    server.server = serverConstructor(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 255, launch);
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


}
















