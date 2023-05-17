#pragma once

#include "Packet.hpp"
#include "Host.hpp"
#include <vector>

class Lobby {
 public:
  Lobby(port_t lobby_port, port_t game_server_port);

  void Run();

 private:
  void InitHost();

  void HandleClients();

  void HandleConnection(const ENetEvent& event);

  void HandleReceivedPacket(const ENetEvent& event);

 private:
  void HandleSessionStartPacket(ENetPeer* sender_peer);

  void SendGameServerPort(ENetPeer* client_peer);

 private:
  bool IsEventFromGameServer(const ENetEvent& event);

  bool IsEventFromClient(const ENetEvent& event);

 private:
  static void LogGameSessionStart(ENetPeer* starter);

  static void LogGameServerPortSending(ENetPeer* client);

 private:
  port_t lobby_port_ = 0;
  port_t game_server_port_ = 0;

  ENetHost* host_ = nullptr;

  std::vector<ENetPeer*> clients_;

  size_t id_counter;

 private:
  bool game_started_ = false;

};
