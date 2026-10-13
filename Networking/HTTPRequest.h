#ifndef HTTPSERVER_HTTPREQUEST_H
#define HTTPSERVER_HTTPREQUEST_H

#include <stdio.h>
#include "../DataStructures/Dictionary/Dictionary.h"
#include "./Nodes/Server.h"

// enum HTTPMethods {
//     GET,
//     POST,
//     PUT,
//     HEAD,
//     PATCH,
//     DELETE,
//     CONNECT,
//     OPTIONS,
//     TRACE,
// };

typedef struct HTTPRequest {
    //switch over to these three
    struct Dictionary requestLine;
    struct Dictionary headerFields;
    struct Dictionary body;

    // int Method;
    // char *URI;
    // float HTTPVersion;
}HTTPRequest;

//maybe we need to get a launch function here
void launch(Server *server);
HTTPRequest httpRequestConstructor(char *requestString);
void httpRequestDestructor(HTTPRequest *request);

#endif //HTTPSERVER_HTTPREQUEST_H