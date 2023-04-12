cmake_minimum_required(VERSION 3.16.3)

project(vagario)

add_executable(
  client
)

add_executable(
  server
)

target_sources(client
  src/client/main.cpp
  src/client/app.cpp
)

target_sources(server
  src/server/server.cpp
)

add_library(network STATIC
)

target_sources(network
  src/network/BitStream.cpp
  src/network/protocol.cpp
)


# includes
target_include_directories(client PUBLIC
  inc
)

target_include_directories(server PUBLIC
  inc
)

target_include_directories(network PUBLIC
  inc
)

# libraries linking
target_link_libraries()