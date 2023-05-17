#include "Client.hpp"

Client::Client(port_t lobby_server_port)
    : lobby_server_port_(lobby_server_port) {
}

void Client::Run() {
  InitHost();
  ConnectToLobbyServer();
  std::thread input_thread(&Client::Input, this);
  ProcessLobbyServerEvents();
}

void Client::InitHost() {
  client_host_ = enet_host_create(nullptr, 2, 2, 0, 0);
  if (client_host_ == nullptr) {
    throw std::runtime_error("cannot create enet host\n");
  }
}

void Client::ConnectToLobbyServer() {
  ENetAddress lobby_address;
  enet_address_set_host(&lobby_address, "localhost");
  lobby_address.port = lobby_server_port_;

  lobby_server_peer_ = enet_host_connect(client_host_, &lobby_address, 2, 0);
  if (lobby_server_peer_ == nullptr) {
    throw std::runtime_error("cannot connect to lobby\n");
  }
  state_ = State::CONNECTED_TO_LOBBY_SERVER;
}

void Client::ProcessLobbyServerEvents() {
  while (state_ != State::CONNECTED_TO_GAME_SERVER) {
    ENetEvent event;
    while (enet_host_service(client_host_, &event, 10) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          LogConnectionEstablished(event.peer->address);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          {
            Packet packet(event.packet);
            switch (packet.type) {
              case PacketType::SERVER_PORT:
                game_server_port_ = *(port_t*)packet.GetData();
                ConnectToGameServer();
                break;
              default:
                throw std::runtime_error("unknown packet type");
                break;
            }
            break;
          }
        case ENET_EVENT_TYPE_DISCONNECT:
          LogDisconnected(event.peer->address);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
  }
}

void Client::Input() {
  while (game_server_peer_ == nullptr) {
    std::string input;
    std::getline(std::cin, input);
    if (input == "start") {
      printf("Starting the game...\n");
      auto start_game = PacketType::SESSION_START;
      auto packet = enet_packet_create(&start_game, sizeof(PacketType),
                                       ENET_PACKET_FLAG_RELIABLE);
      enet_peer_send(lobby_server_peer_, 0, packet);
      break;
    }
  }
}

void Client::ConnectToGameServer() {
  ENetAddress address;
  enet_address_set_host(&address, "localhost");
  address.port = game_server_port_;

  game_server_peer_ = enet_host_connect(client_host_, &address, 2, 0);
  state_ = State::CONNECTED_TO_GAME_SERVER;
}

void Client::ProcessGameServerEvents() {
  while (true) {

  }
}

int main() {
  Client client(10887);
  client.Run();
  return 0;
}
