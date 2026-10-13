cmake_minimum_required(VERSION 4.0)
project(HTTPServer C)

set(CMAKE_C_STANDARD 23)

add_executable(HTTPServer main.c
        Networking/Nodes/Server.c
        Networking/Nodes/Server.h
        Networking/test.c
        Networking/HTTPRequest.c
        Networking/HTTPRequest.h
        DataStructures.h
        DataStructures/Common/Node.c
        DataStructures/Dictionary/Dictionary.c
        DataStructures/Dictionary/Entry.c
        DataStructures/Lists/LinkedList.c
        DataStructures/Lists/Queue.c
        DataStructures/Trees/BinarySearchTree.c
        Networking/Nodes/HTTPServer.c
        Networking/Nodes/HTTPServer.h)

target_link_libraries(HTTPServer PRIVATE ws2_32)
