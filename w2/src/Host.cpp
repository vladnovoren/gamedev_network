#include "Host.hpp"

void LogConnectionEstablished(const ENetAddress& address) {
  printf("Connection established with:\n"
         "port: %d\n"
         "host: %d\n", address.host, address.port);
}

void LogDisconnected(const ENetAddress& address) {
  printf("Disconnected from:\n"
        "port: %d\n"
        "host: %d\n", address.host, address.port);
}
