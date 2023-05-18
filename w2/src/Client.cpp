#include "Client.hpp"

Client::Client(port_t lobby_server_port)
    : lobby_server_port_(lobby_server_port) {
}

void Client::Run() {
  InitHost();
  ConnectToLobbyServer();
  HandleEvents();
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

void Client::ConnectToGameServer() {
  ENetAddress address;
  enet_address_set_host(&address, "localhost");
  address.port = game_server_port_;

  game_server_peer_ = enet_host_connect(client_host_, &address, 2, 0);
  state_ = State::CONNECTED_TO_GAME_SERVER;
}

void Client::HandleEvents() {
  while (true) {
    ENetEvent event;
    while (enet_host_service(client_host_, &event, 10) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          LogConnectionEstablished(event.peer->address);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          HandlePacketReceiving(event);
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          LogDisconnected(event.peer->address);
          break;
        case ENET_EVENT_TYPE_NONE:
          break;
      }
    }
    Input();
    if (state_ == State::CONNECTED_TO_GAME_SERVER) {
      SendTimeToGameServer();
    }
  }
}

void Client::HandlePacketReceiving(const ENetEvent& event) {
  if (IsEventFromPeer(event, lobby_server_peer_)) {
    HandleLobbyServerPacket(Packet(event.packet));
  } else if (IsEventFromPeer(event, game_server_peer_)) {
    HandleGameServerPacket(Packet(event.packet));
  } else {
    throw std::runtime_error("packet received from unexpected peer");
  }
}

void Client::HandleLobbyServerPacket(const Packet& packet) {
  switch (packet.type) {
    case PacketType::SERVER_PORT:
      game_server_port_ = *(port_t*)packet.GetData();
      ConnectToGameServer();
      break;
    default:
      throw std::runtime_error("unexpected packet type");
      break;
  }
}

void Client::HandleGameServerPacket(const Packet& packet) {
  switch (packet.type) {
    case PacketType::MESSAGE:
      std::cout << (char*)packet.GetData() << '\n';
      break;
    default:
      throw std::runtime_error("unexpected packet type");
      break;
  }
}

void Client::SendTimeToGameServer() {
  static IntervalTimer timer(1000);
  if (timer.IsReset()) {
    std::string msg = "time is ";
    msg += std::to_string(enet_time_get());
    Packet packet(PacketType::MESSAGE, (byte_t*)msg.c_str(), msg.size() + 1);
    enet_peer_send(game_server_peer_, 0, packet.enet_packet);
  }
}

void Client::Input() {
  if (game_started_)
    return;

  std::string input;

  fd_set readSet;
  FD_ZERO(&readSet);
  FD_SET(STDIN_FILENO, &readSet);

  timeval timeout = {0, 10000};
  select(STDIN_FILENO + 1, &readSet, NULL, NULL, &timeout);

  if (FD_ISSET(STDIN_FILENO, &readSet)) {
    std::getline(std::cin, input);
    if (input  == "start") {
      printf("Starting the game...\n");
      Packet packet(PacketType::SESSION_START);
      enet_peer_send(lobby_server_peer_, 0, packet.enet_packet);
      game_started_ = true;
    }
  }
}

int main() {
  Client client(2001);
  client.Run();
  return 0;
}
