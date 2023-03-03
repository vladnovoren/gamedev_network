cmake_minimum_required(VERSION 3.16.3)

project(lobby)

add_executable(lobby
    src/lobby.cpp
)

add_executable(client
    src/client.cpp
)
