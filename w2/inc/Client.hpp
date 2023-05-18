#pragma once

#include "Packet.hpp"
#include "Host.hpp"
#include "Time.hpp"
#include <stdexcept>
#include <thread>

class Client {
 public:
  Client(port_t lobby_server_port);

  void Run();

 private:
  void InitHost();

  void Input();

  void ConnectToLobbyServer();

  void ConnectToGameServer();

 private:
  void HandleEvents();

  void HandlePacketReceiving(const ENetEvent& event);

  void HandleLobbyServerPacket(const Packet& packet);

  void HandleGameServerPacket(const Packet& packet);

 private:
  void SendTimeToGameServer();

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

  bool game_started_ = false;

  State state_ = State::IDLE;
};
