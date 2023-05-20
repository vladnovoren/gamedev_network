cmake_minimum_required(VERSION 3.16.3)

project(lobby)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(COMPILE_OPTIONS
  "-O2
  -g
  -fsanitize=address

  -Wall
  -Wextra
  -Wpedantic
  -Werror"
)

set(LINK_OPTIONS "-fsanitize=address")


add_subdirectory(${CMAKE_SOURCE_DIR}/../third_party/enet
${CMAKE_SOURCE_DIR}/../third_party/enet/lib)


add_executable(client)

target_sources(client PUBLIC
  src/Client.cpp
)

target_compile_options(client PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")
target_link_options(client PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")

add_executable(lobby)

target_sources(lobby PUBLIC
  src/Lobby.cpp
)

target_compile_options(lobby PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")
target_link_options(lobby PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")

add_executable(game)

target_sources(game PUBLIC
  src/Game.cpp
)

target_compile_options(game PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")
target_link_options(game PUBLIC "$<$<CONFIG:DEBUG>:${COMPILE_OPTIONS}>")

add_library(network STATIC)

target_sources(network PUBLIC
  src/Packet.cpp
  src/Host.cpp
  src/Time.cpp
)


target_link_libraries(network PUBLIC enet)
target_link_libraries(client PUBLIC network)
target_link_libraries(lobby PUBLIC network)
target_link_libraries(game PUBLIC network)

target_include_directories(network PUBLIC inc)
target_include_directories(client PUBLIC inc)
target_include_directories(lobby PUBLIC inc)
target_include_directories(game PUBLIC inc)
