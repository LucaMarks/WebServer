#ifndef HTTPSERVER_HTTPREQUEST_H
#define HTTPSERVER_HTTPREQUEST_H

#include <stdio.h>
#include "../DataStructures/Dictionary/Dictionary.h"

enum HTTPMethods {
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
};

typedef struct HTTPRequest {
    int Method;
    char *URI;
    float HTTPVersion;
    struct Dictionary headerFields;
}HTTPRequest;

HTTPRequest httpRequestConstructor(char *requestString);

#endif //HTTPSERVER_HTTPREQUEST_H