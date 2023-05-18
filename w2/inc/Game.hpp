#pragma once

#include "Packet.hpp"
#include "Host.hpp"
#include <vector>
#include <unordered_map>

class Game {
 public:
  Game(port_t port);

  void Run();

 private:
  class Client {
   public:
    Client() = default;
    Client(ENetPeer* peer);

    const std::string& Name() const;
    size_t Id() const;

   public:
    ENetPeer* peer;

   private:
    size_t id_;
    std::string name_;

   private:
    static size_t id_counter;
  };

 private:
  void InitHost();

  void HandleEvents();

  void HandleConnection(ENetPeer* peer);

  void HandleDisconnect(ENetPeer* peer);

 private:
  void SendPlayerList(Client& new_client);

  void SendNewPlayerInfo(Client& client);

 private:
  ENetHost* host_;

  std::unordered_map<size_t, Client> clients_;
  std::unordered_map<port_t, size_t> port_id_;

  port_t game_server_port_;
};
