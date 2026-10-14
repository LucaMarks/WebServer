#include "Server.h"
#include <stdio.h>
#include <stdlib.h>

struct Server serverConstructor(int domain, int service, int protocol, int port, u_long interface, int backlog /*void(*launch)(Server *server)*/) {
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);

    if (server.socket == 0) {
        perror("Failed to connect to socket\n");
        exit(1);
    }


    // int *arr = (int*) malloc(5 * sizeof(int));


    if ((bind(server.socket, (struct sockaddr*)&server.address, sizeof(server.address))) < 0) {
        perror("Failed to bind socket\n");
        exit(1);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening\n");
        exit(1);
    }

    // server.launch = launch;

    return server;
}
