#include "Server.hpp"
#include <stdexcept>

Server::Server(port_t port) : server_host_port_(port) {
}

void Server::Run() {
  Init();
  MainLoop();
}

void Server::Init() {
  InitHost();
  InitAIEntities();
}

void Server::InitHost() {
  if (enet_initialize() != 0)  {
    throw std::runtime_error("can't init ENet");
  }
  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = server_host_port_;
  server_host_ = enet_host_create(&address, 32, 2, 0, 0);
  if (server_host_ == nullptr) {
    throw std::runtime_error("can't create ENet server\n");
  }
}

void Server::InitAIEntities() {
  for (eid_t i = 0; i < n_ai_entities_; ++i) {
    Entity entity;
    auto color = GetRandomColor();
    auto position = GetRandomPosition();
    auto radius = GetRandomRadius();
    entities_.emplace_back(color, position, radius, i);
    ai_targets_.emplace_back(GetRandomPosition());
  }
}

void Server::MainLoop() {
  clock_t prev_time = clock();
  while (true) {
    float curr_time = clock();
    float dt = (curr_time - prev_time) / CLOCKS_PER_SEC;
    prev_time = curr_time;
    ENetEvent event;
    while (enet_host_service(server_host_, &event, 10) > 0) {
      switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
          LogConnectionEstablished(event.peer->address);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          HandleReceivedPacket(event);
          break;
        default:
          break;
      }
    }
    MoveAIEntities(dt);
    HandleCollisions();
    SendSnapshots();
  }
}

void Server::HandleReceivedPacket(ENetEvent& event) {
  switch (get_packet_type(event.packet)) {
    case E_CLIENT_TO_SERVER_JOIN:
      HandleJoinPacket(event);
      break;
    case E_CLIENT_TO_SERVER_STATE:
      HandleStatePacket(event);
      break;
    default:
      throw std::runtime_error("unexpected packet type");
      break;
  }
}

void Server::HandleJoinPacket(ENetEvent& event) {
  SendAllEntities(event.peer);

  entities_.emplace_back(
      GetRandomColor(),
      GetRandomPosition(),
      GetRandomRadius(),
      entities_.size()
  );

  SendNewEntityToOthers(entities_.back());
  send_set_controlled_entity(event.peer, entities_.back().eid);

  eid_to_player_peer_map_[entities_.back().eid] = event.peer;
}

void Server::HandleStatePacket(ENetEvent& event) {
  eid_t eid = invalid_entity;
  float x = 0.f,
        y = 0.f;
  deserialize_entity_state(event.packet, eid, x, y);
  for (Entity& entity : entities_) {
    if (entity.eid == eid) {
      entity.position = {x, y};
    }
  }
}

void Server::SendAllEntities(ENetPeer* peer) {
  for (const auto& entity : entities_) {
    send_new_entity(peer, entity);
  }
}

void Server::SendNewEntityToOthers(const Entity& new_entity) {
  for (size_t i = 0; i < server_host_->peerCount; ++i) {
    send_new_entity(&server_host_->peers[i], new_entity);
  }
}

void Server::SendSnapshots() {
  for (size_t i = 0; i < server_host_->peerCount; ++i) {
    ENetPeer* peer = &server_host_->peers[i];
    for (const auto& entity : entities_) {
      if (eid_to_player_peer_map_[entity.eid] != peer ||
          collided_[entity.eid]) {
          send_snapshot(peer, entity.eid,
              entity.position.x, entity.position.y, entity.radius);
      }
    }
  }
}

void Server::MoveAIEntities(float dt) {
  for (eid_t i = 0; i < n_ai_entities_; ++i) {
    Vec2f to_target = ai_targets_[i] - entities_[i].position;
    float ds = speed_ * dt;
    float dist_to_target = to_target.Length();
    if (dist_to_target < ds) {
      entities_[i].position = ai_targets_[i];
      ai_targets_[i] = GetRandomPosition();
    } else {
      entities_[i].position += (dt * speed_ / dist_to_target) * to_target;
    }
  }
}

void Server::HandleCollisions() {
  collided_.assign(entities_.size(), false);
  for (auto& a : entities_) {
    for (auto& b : entities_) {
      if (a.radius < b.radius &&
          a.position.DistTo(b.position) < a.radius + b.radius) {
        b.radius = std::sqrt(b.radius * b.radius + a.radius * a.radius / 2.f);
        a.radius /= std::sqrt(2);
        if (a.radius < min_radius_) {
          a.radius = GetRandomRadius();
        }
        a.position = GetRandomPosition();
        if (eid_to_player_peer_map_[a.eid] == nullptr) {
          ai_targets_[a.eid] = GetRandomPosition();
        }
        collided_[a.eid] = collided_[b.eid] = true;
      }
    }
  }
}

uint32_t Server::GetRandomColor() {
  return 0x000000ff +
         ((rand() % 256) << 8) +
         ((rand() % 256) << 16) +
         ((rand() % 256) << 24);
}

Vec2f Server::GetRandomPosition() {
  return {
    -width_ / 2 + (float)(rand() % (int)width_),
    -height_ / 2 + (float)(rand() % (int)height_)
  };
}

float Server::GetRandomRadius() {
  return min_radius_ + rand() % (int)max_add_radius_;
}

int main() {
  try {
    Server server(34363);
    server.Run();
  } catch (std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }
}
