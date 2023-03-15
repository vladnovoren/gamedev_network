#pragma once

#include <cassert>
#include <enet/enet.h>
#include "Packet/Packet.hpp"
#include <utility>

class Peer {
 public:
  int Send(const Packet& packet);

 private:
  explicit Peer(ENetPeer* enet_peer);

 private:
  static constexpr char const* ENET_PACKET_CREATION_BAG_NSG = "cannot create package";
  static constexpr char const* ENET_PACKET_SEND_BAG_MSG = "cannot send package";

 private:
  ENetPeer* enet_peer_ = nullptr;

  friend class Host;
};
