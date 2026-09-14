#include "HTTPRequest.h"

#include <stdlib.h>
#include <string.h>
#include "../DataStructures/Lists/Queue.h"

int methodSelect(char *method) {
    if (strcmp(method, "GET") == 0) {
        return GET;
    }
    else if (strcmp(method, "POST") == 0) {
        return POST;
    }
    else if (strcmp(method, "PUT") == 0) {
        return PUT;
    }
    else if (strcmp(method, "HEAD") == 0) {
        return HEAD;
    }
    else if (strcmp(method, "PATCH") == 0) {
        return PATCH;
    }
    else if (strcmp(method, "DELETE") == 0) {
        return DELETE;
    }
    else if (strcmp(method, "CONNECT") == 0) {
        return CONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0) {
        return OPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0) {
        return TRACE;
    }
}

HTTPRequest httpRequestConstructor(char *requestString) {

    char requested[strlen(requestString)];
    strcpy(requested, requestString);

    for (int i = 0; i < strlen(requested) -1; i++) {
        if (requested[i] == '\n' && requested[i + 1] == '\n') {
            requested[i+1] = '|';
        }
    }

    char *requestLine = strtok(requested, "\n");
    char *headerFields = strtok(NULL, "|");
   char *body = strtok(NULL, "|");

    HTTPRequest request;

    char *method = strtok(requestLine," ");
    request.Method = methodSelect(method);
    char *URI = strtok(NULL, " ");
    request.URI = URI;
    char *HTTPVersion = strtok(NULL, " ");
    HTTPVersion = strtok(HTTPVersion, "/");
    HTTPVersion = strtok(NULL, "/");
    request.HTTPVersion = (float)atof(HTTPVersion);

    request.headerFields = dictionary_constructor(compare_string_keys);

    struct Queue headers = queue_constructor();

    char *token = strtok(headerFields, "\n");
    while (token) {
        headers.push(&headers, token, sizeof(*token));
        token = strtok(NULL, "\n");
    }

    char *header = (char *)headers.peek(&headers);
    while (header) {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "|");
        request.headerFields.insert(&request.headerFields, key, sizeof(*key), value, sizeof(*value));
        headers.pop(&headers);
        header = (char *)headers.peek(&headers);

    }


    return request;
}