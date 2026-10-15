#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif


typedef struct Server {
    int domain;
    int protocol;
    int service;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    // void (*launch)(struct Server *server);

}Server;

struct Server serverConstructor(int domain, int service, int protocol, int port, u_long interface, int backlog /*void(*launch)(Server *server)*/);

#endif
