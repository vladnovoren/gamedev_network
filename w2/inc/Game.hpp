#pragma once

#include "Packet.hpp"
#include "Host.hpp"
#include <vector>

class Game {
 public:
  Game(port_t port);

  void Run();

 private:
  class Client {
   public:
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

 private:
  void SendPlayerList(Client& new_client);

 private:
  ENetHost* host_;

  std::vector<Client> clients_;

  port_t game_server_port_;
};
