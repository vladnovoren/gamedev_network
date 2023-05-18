#include "Lobby.hpp"
#include <stdexcept>

Lobby::Lobby(port_t lobby_port, port_t game_server_port) :
  lobby_port_(lobby_port), game_server_port_(game_server_port) {
}

void Lobby::Run() {
  InitHost();
  HandleClients();
}

void Lobby::InitHost() {
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = lobby_port_;
  host_ = enet_host_create(&address, 32, 2, 0, 0);
}

void Lobby::HandleClients() {
  while (true) {
    ENetEvent event;
    while (enet_host_service(host_, &event, 10) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          HandleConnection(event);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          HandleReceivedPacket(event);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          LogDisconnected(event.peer->address);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }
}

void Lobby::HandleConnection(const ENetEvent& event) {
  LogConnectionEstablished(event.peer->address);
  if (IsEventFromClient(event)) {
    if (game_started_) {
      SendGameServerPort(event.peer);
    }
    clients_.push_back(event.peer);
  }
}

void Lobby::HandleReceivedPacket(const ENetEvent& event) {
  if (IsEventFromClient(event)) {
    Packet packet(event.packet);
    switch (packet.type) {
      case PacketType::SESSION_START:
        HandleSessionStartPacket(event.peer);
        break;
      default:
        throw std::runtime_error("unexpected packet type");
    }
  }
}

void Lobby::HandleSessionStartPacket(ENetPeer* sender_peer) {
  assert(sender_peer != nullptr);

  if (!game_started_) {
    game_started_ = true;
    LogGameSessionStart(sender_peer);
    for (auto peer : clients_) {
      SendGameServerPort(peer);
    }
  }
}

void Lobby::SendGameServerPort(ENetPeer* client_peer) {
  Packet packet(PacketType::SERVER_PORT, (byte_t*)&game_server_port_,
                sizeof(port_t));
  std::cout << *(int*)packet.enet_packet->data << '\n';
  std::cout << *(port_t*)(packet.enet_packet->data + sizeof(PacketType)) << '\n';
  std::cout << packet.enet_packet->dataLength << '\n';
  enet_peer_send(client_peer, 0, packet.enet_packet);
  LogGameServerPortSending(client_peer);
}

bool Lobby::IsEventFromGameServer(const ENetEvent& event) {
  return event.peer->address.port == game_server_port_;
}

bool Lobby::IsEventFromClient(const ENetEvent& event) {
  return !IsEventFromGameServer(event);
}

void Lobby::LogGameSessionStart(ENetPeer* starter) {
  printf("client[port = %d] starts game session\n", starter->address.port);
}

void Lobby::LogGameServerPortSending(ENetPeer* client) {
  printf("game server port sent to client[port = %d]\n", client->address.port);
}

int main() {
  Lobby lobby(10887, 10888);
  lobby.Run();
  return 0;
}
