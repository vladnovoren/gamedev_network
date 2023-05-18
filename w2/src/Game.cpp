#include "Game.hpp"
#include <stdexcept>

size_t Game::Client::id_counter = 0;

Game::Client::Client(ENetPeer* in_peer) : peer(in_peer), id_(id_counter++),
  name_("player_" + std::to_string(id_)) {
}

const std::string& Game::Client::Name() const {
  return name_;
}

size_t Game::Client::Id() const {
  return id_;
}

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
          HandleConnection(event.peer);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          HandleDisconnect(event.peer);
          break;
        default:
          throw std::runtime_error("unknown enet event type");
          break;
      }
    }
  }
}

void Game::HandleConnection(ENetPeer* peer) {
  assert(peer != nullptr);

  LogConnectionEstablished(peer->address);
  Client new_client(peer);
  SendPlayerList(new_client);
  SendNewPlayerInfo(new_client);
  clients_[new_client.Id()] = new_client;
  port_id_[peer->address.port] = new_client.Id();
}

void Game::HandleDisconnect(ENetPeer* peer) {
  assert(peer != nullptr);

  LogDisconnected(peer->address);

  auto id = port_id_[peer->address.port];
  auto client_it = clients_.find(id);
  std::string name = client_it->second.Name();
  clients_.erase(id);

  std::string msg = "Player ";
  msg += name;
  msg += " leaved game";

  for (auto& client : clients_) {
    SendMessage(client.second.peer, msg);
  }
}

void Game::SendPlayerList(Client& new_client) {
  std::string message;

  if (clients_.empty()) {
    message = "No players on server yet";
  } else {
    message = "Player list:\n";
    for (auto& client : clients_) {
      message += client.second.Name();
      message += '\n';
    }
  }

  SendMessage(new_client.peer, message);
}

void Game::SendNewPlayerInfo(Client& new_client) {
  std::string message = "New player joined game: ";
  message += new_client.Name();

  for (auto& client : clients_) {
    SendMessage(client.second.peer, message);
  }
}

void Game::InitHost() {
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = game_server_port_;
  host_ = enet_host_create(&address, 32, 2, 0, 0);
}

int main() {
  Game game(2002);
  game.Run();
  return 0;
}