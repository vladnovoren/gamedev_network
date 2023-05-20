#include "Host.hpp"

bool IsEventFromPeer(const ENetEvent& event, const ENetPeer* peer) {
  return event.peer->address.host == peer->address.host &&
         event.peer->address.port == peer->address.port;
}

void LogConnectionEstablished(const ENetAddress& address) {
  printf("Connection established with: [host = %d, port = %d]\n",
         address.host, address.port);
}

void LogDisconnected(const ENetAddress& address) {
  printf("Disconnected from: [host = %d, port = %d]\n",
         address.host, address.port);
}

