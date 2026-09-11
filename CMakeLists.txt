cmake_minimum_required(VERSION 4.0)
project(HTTPServer C)

set(CMAKE_C_STANDARD 23)

add_executable(HTTPServer main.c
        Networking/server.c
        Networking/server.h
        Networking/test.c)

target_link_libraries(HTTPServer PRIVATE ws2_32)
