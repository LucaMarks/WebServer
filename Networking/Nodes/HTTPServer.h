#ifndef HTTPSERVER_HTTPSERVER_H
#define HTTPSERVER_HTTPSERVER_H

#include "Server.h"
#include "../Protocols/HTTPRequest.h"

typedef struct HTTPServer {

    struct Server server;
    struct Dictionary routes;
    void (*registerRoutes)(struct HTTPServer *server, void (*routeFunction)(struct HTTPServer *server, HTTPRequest *request), char *uri, int numMethods, ...);

}HTTPServer;

HTTPServer httpServerConstructor(void);

void launch(HTTPServer *);

#endif //HTTPSERVER_HTTPSERVER_H