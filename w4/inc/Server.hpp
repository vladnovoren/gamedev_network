#pragma once

#include "entity.h"
#include "protocol.h"
#include <iostream>
#include <vector>
#include <unordered_map>

class Server {
 public:
  Server(port_t port);

  void Run();

 private:
  void Init();

  void InitHost();
  void InitAIEntities();

  void MainLoop();

 private:
  void HandleReceivedPacket(ENetEvent& event);

  void HandleJoinPacket(ENetEvent& event);
  void HandleStatePacket(ENetEvent& event);

 private:
  void SendAllEntities(ENetPeer* peer);
  void SendNewEntityToOthers(const Entity& new_entity);
  void SendSnapshots();

 private:
  void MoveAIEntities(float dt);
  void HandleCollisions();

  uint32_t GetRandomColor();
  Vec2f GetRandomPosition();
  float GetRandomRadius();

 private:
  static constexpr size_t n_ai_entities_ = 10;
  static constexpr float speed_ = 2000;
  static constexpr float min_radius_ = 10;
  static constexpr float max_add_radius_ = 20;
  static constexpr float width_ = 600;
  static constexpr float height_ = 600;

  ENetHost* server_host_ = nullptr;
  port_t server_host_port_;

  std::vector<Entity> entities_;
  std::vector<Vec2f> ai_targets_;
  std::vector<bool> collided_;
  std::unordered_map<eid_t, ENetPeer*> eid_to_player_peer_map_;

};
