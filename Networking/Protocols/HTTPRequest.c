#include "HTTPRequest.h"
// #include "../Nodes/HTTPServer.h"

#include <stdlib.h>
#include <string.h>
#include "../../DataStructures/Lists/Queue.h"

void extractRequestLineFields(HTTPRequest *request, char *requestLine);
void extractHeaderFields(HTTPRequest *request, char *headerFields);
void extractBody(HTTPRequest *request, char *body);

int methodSelect(char *method) {
    if (strcmp(method, "GET") == 0) {
        return HTTP_GET;
    }
    else if (strcmp(method, "POST") == 0) {
        return HTTP_POST;
    }
    else if (strcmp(method, "PUT") == 0) {
        return HTTP_PUT;
    }
    else if (strcmp(method, "HEAD") == 0) {
        return HTTP_HEAD;
    }
    else if (strcmp(method, "PATCH") == 0) {
        return HTTP_PATCH;
    }
    else if (strcmp(method, "DELETE") == 0) {
        return HTTP_DELETE;
    }
    else if (strcmp(method, "CONNECT") == 0) {
        return HTTP_CONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0) {
        return HTTP_OPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0) {
        return HTTP_TRACE;
    }
}

HTTPRequest httpRequestConstructor(char *requestString) {

    char requested[strlen(requestString)+1];
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

    //parse each section as needed
    extractRequestLineFields(&request, requestLine);
    extractHeaderFields(&request, headerFields);
    extractBody(&request, body);

    return request;

    }



void extractRequestLineFields(HTTPRequest *request, char *requestLine) {
    //copy the string literal into a local instance
    char fields[strlen(requestLine)];
    strcpy(fields, requestLine);

    //separate the string on spaces for each section
    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *httpVersion = strtok(NULL, " ");

    //insert the results into the request object as a dictionary

    struct Dictionary requestLineDict = dictionary_constructor(compare_string_keys);
    requestLineDict.insert(&requestLineDict, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
    requestLineDict.insert(&requestLineDict, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
    requestLineDict.insert(&requestLineDict, "httpVersion", sizeof("httpVersion"), httpVersion, sizeof(char[strlen(httpVersion)]));

    //save the dictionary to the request object
    (*request).requestLine = requestLineDict;

    if (request->requestLine.search(&(request->requestLine), "GET", sizeof("GET"))) {
        extractBody(request, (char *)request->requestLine.search(&(request->requestLine), "uri", sizeof("uri")));

    }
}

//parses out the header fields
void extractHeaderFields(HTTPRequest *request, char *headerFields) {

    //copy the literal into a local instance
    char fields[strlen(headerFields)+1];
    strcpy(fields, headerFields);
    //save each line of the input into a queue
    struct Queue headers = queue_constructor();
    char *field = strtok(fields, "\n");

    while (field) {
        headers.push(&headers, field, sizeof(char[strlen(field)])+1);
        field = strtok(NULL, "\n");
    }

    //initialize the request's headerFields dict
    request->headerFields = dictionary_constructor(compare_string_keys);

    //use the queue to further extract key-value pairs
    char *header = (char *)headers.peek(&headers);
    while (header) {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\0");
        if (value) {
            //remove leading space
            if (value[0] == ' '){value++;}

            //push key-value pairs into request's headerFields dict
            request->headerFields.insert(&(request->headerFields), key, sizeof(char[strlen(key)])+1, value, sizeof(char[strlen(value)])+1);
            //collect the next field from queue
        }
        headers.pop(&headers);
        header = (char*)headers.peek(&headers);
    }

    //destroy the queue
    queue_destructor(&headers);

}

void extractBody(HTTPRequest *request, char *body) {

    //check what content type needs to be parsed
    char *contentType = (char *)request->headerFields.search(&(request->headerFields), "Content-Type", sizeof("Content-Type"));
    if (contentType) {
        //initialize the bodyFields dictionary
        struct Dictionary bodyFields = dictionary_constructor(compare_string_keys);
        if (strcmp(contentType, "application/x-www-form-urlencoded") == 0) {
            //collect each key-value pair as a set and store them in a queue
            struct Queue fields = queue_constructor();
            char *field = strtok(body, "&");

            while (field) {
                fields.push(&fields, field, sizeof(char[strlen(field)]));
            }
            //iterate over the queue to further separate fields from values
            field = fields.peek(&fields);
            while (field) {
                char *key = strtok(field, "=");
                char *value = strtok(NULL, "\0");
                //remove leading white space

                if (value[0] == ' ') {
                    value++;
                }

                //insert the key-value pair into the dictionary
                bodyFields.insert(&bodyFields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
                //collect the next item in the queue
                fields.pop(&fields);
                field = fields.peek(&fields);
            }
            //destroy the queue
            queue_destructor(&fields);
        }else {
            //save the data as a single key-value pair
            bodyFields.insert(&bodyFields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        //set the request's body dictionary
        request->body = bodyFields;
    }
}

