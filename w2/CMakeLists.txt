cmake_minimum_required(VERSION 3.16.3)

project(lobby)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(
        -O2
        -g
        -fsanitize=address

        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

add_link_options(
        -fsanitize=address
)

include(${PROJECT_SOURCE_DIR}/FindENet.cmake)

add_executable(client
        src/Client/main.cpp
        src/Client/Client.cpp
        )

add_executable(lobby_server
        src/lobby_server/main.cpp
        src/lobby_server/LobbyServer.cpp
)

add_executable(game_server
        src/game_server/main.cpp
)

add_executable(demo_connect_client
        src/demos/connect/client.cpp
)

add_executable(demo_connect_server
        src/demos/connect/server.cpp
)

add_library(network STATIC
        src/network/Packet/Packet.cpp
        src/network/Packet/PacketData.cpp

        src/network/Peer.cpp

        src/network/Host/Host.cpp
        src/network/Host/Events.cpp
        )

message(${ENet_LIBRARIES})

target_link_libraries(network PUBLIC ${ENet_LIBRARIES})
target_link_libraries(client PUBLIC network)
target_link_libraries(lobby_server PUBLIC network)
target_link_libraries(game_server PUBLIC network)

target_link_libraries(demo_connect_client PUBLIC network)
target_link_libraries(demo_connect_server PUBLIC network)

include_directories(inc ${ENet_INCLUDE_DIRS})