#ifndef HTTPSERVER_HTTPSERVER_H
#define HTTPSERVER_HTTPSERVER_H

#include "Server.h"
#include "../HTTPRequest.h"

typedef struct HTTPServer {
    //Public member variables
    Server server;
    //dictionary of server routes registered on the server with the URL's as keys
    struct Dictionary routes;

    //public member methods
    //register url's as routes to the server
    void (*registerRoutes)(struct HTTPServer *server, char *(*routeFunction)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int numMethods, ...);
    //launch sequence acts as a listener where the server listens for, and hanldes incoming connections
    void (*launch)(struct HTTPServer *server);

}HTTPServer;

enum HTTPMethods {
    HTTP_CONNECT,
    HTTP_DELETE,
    HTTP_GET,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH,
    HTTP_POST,
    HTTP_PUT,
    HTTP_TRACE
};

//constructor
//struct isn't necessary here
struct HTTPServer httpServerConstructor(void);

//helper functions
char *renderTemplates(int numTemplates, ...);


#endif //HTTPSERVER_HTTPSERVER_H