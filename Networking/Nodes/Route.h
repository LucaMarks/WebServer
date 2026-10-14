#ifndef HTTPSERVER_ROUTE_H
#define HTTPSERVER_ROUTE_H

#include "HTTPServer.h"
#include "../Protocols/HTTPRequest.h"

typedef struct Route {
    int methods[9];//since HTTP allows for 9 different protocols
    char *uri;
    char *(*routeFunction)(HTTPServer *, HTTPRequest *);
}Route;

#endif //HTTPSERVER_ROUTE_H