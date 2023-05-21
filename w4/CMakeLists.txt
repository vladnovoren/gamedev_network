cmake_minimum_required(VERSION 3.13)

project(client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIRECTORY}../third_party/enet
${CMAKE_SOURCE_DIRECTORY}../third_party/lib)
add_subdirectory(${CMAKE_SOURCE_DIRECTORY}../third_party/raylib
${CMAKE_SOURCE_DIRECTORY}../third_party/raylib/lib)

set(client_sources
    src/Client.cpp
)

set(server_sources
    src/Server.cpp
)

set(network_sources
  src/IBitStream.cpp
  src/OBitStream.cpp
  src/protocol.cpp
)

add_library(network STATIC ${network_sources})
target_link_libraries(network PUBLIC enet)

if(MSVC)
  # https://github.com/raysan5/raylib/issues/857
  add_compile_definitions(NOVIRTUALKEYCODES NOWINMESSAGES NOWINSTYLES NOSYSMETRICS NOMENUS NOICONS NOKEYSTATES NOSYSCOMMANDS NORASTEROPS NOSHOWWINDOW OEMRESOURCE NOATOM NOCLIPBOARD NOCOLOR NOCTLMGR NODRAWTEXT NOGDI NOKERNEL NOUSER NOMB NOMEMMGR NOMETAFILE NOMINMAX NOMSG NOOPENFILE NOSCROLL NOSERVICE NOSOUND NOTEXTMETRIC NOWH NOWINOFFSETS NOCOMM NOKANJI NOHELP NOPROFILER NODEFERWINDOWPOS NOMCX)
endif()

add_executable(client ${client_sources})
target_link_libraries(client PUBLIC raylib network)

add_executable(server ${server_sources})
target_link_libraries(server PUBLIC network)

if(MSVC)
  target_link_libraries(client PUBLIC ws2_32.lib winmm.lib)
  target_link_libraries(server PUBLIC ws2_32.lib winmm.lib)
endif()

target_include_directories(client PUBLIC inc)
target_include_directories(server PUBLIC inc)
target_include_directories(network PUBLIC inc)

target_link_libraries(client PUBLIC network)
target_link_libraries(server PUBLIC network)
