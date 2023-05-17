cmake_minimum_required(VERSION 3.16.3)

project(lobby)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

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


add_executable(client)

target_sources(client PUBLIC
  src/Client.cpp
)


add_executable(lobby)

target_sources(lobby PUBLIC
  src/Lobby.cpp
)


add_library(network STATIC)

target_sources(network PUBLIC
        src/Packet.cpp
        src/Host.cpp
)

message(${ENet_LIBRARIES})

target_link_libraries(network PUBLIC ${ENet_LIBRARIES})
target_link_libraries(client PUBLIC network)
target_link_libraries(lobby PUBLIC network)

include_directories(inc ${ENet_INCLUDE_DIRS})
