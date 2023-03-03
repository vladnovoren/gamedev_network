cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)

project(server_client)

add_executable(client
        src/client/main.cpp
        src/client/Client.cpp
)

add_executable(server
        src/server/main.cpp
        src/server/Server.cpp
)

add_library(network STATIC
        src/network/socket_init.cpp
        src/network/DgramSocket.cpp
        src/network/Packet.cpp
)

target_include_directories(client PUBLIC inc)
target_include_directories(server PUBLIC inc)
target_include_directories(network PUBLIC inc)

target_link_libraries(client PUBLIC network)
target_link_libraries(server PUBLIC network)

