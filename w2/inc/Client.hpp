#pragma once

#include "Packet.hpp"
#include "Host.hpp"
#include <stdexcept>
#include <thread>

class Client {
 public:
  Client(port_t lobby_server_port);

  void Run();

 private:
  void InitHost();

  void ConnectToLobbyServer();

  void ProcessLobbyServerEvents();

  void Input();

  void ConnectToGameServer();

  void ProcessGameServerEvents();

 private:
  ENetHost* client_host_ = nullptr;

  ENetPeer* lobby_server_peer_ = nullptr;
  port_t lobby_server_port_;

  ENetPeer* game_server_peer_ = nullptr;
  port_t game_server_port_;
 private:
  enum class State {
    IDLE,
    CONNECTED_TO_LOBBY_SERVER,
    CONNECTED_TO_GAME_SERVER
  };

  State state_ = State::IDLE;
};
