#pragma once

#include "Packet.hpp"
#include "Host.hpp"

class Game {
 public:
  Game(port_t port);

  void Run();

 private:
  void InitHost();

  void HandleEvents();

 private:
  ENetHost* host_;
  port_t game_server_port_;
};
