#include "Game.hpp"
#include <stdexcept>

Game::Game(port_t port) : game_server_port_(port) {
}

void Game::Run() {
  InitHost();
  HandleEvents();
}

void Game::HandleEvents() {
  while (true) {
    ENetEvent event;
    while (enet_host_service(host_, &event, 10) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_NONE:
          break;
        case ENET_EVENT_TYPE_CONNECT:
          LogConnectionEstablished(event.peer->address);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          LogDisconnected(event.peer->address);
          break;
        default:
          throw std::runtime_error("unknown enet event type");
          break;
      }
    }
  }
}

void Game::InitHost() {
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = game_server_port_;
  host_ = enet_host_create(&address, 32, 2, 0, 0);
}

int main() {
  Game game(10888);
  game.Run();
  return 0;
}
