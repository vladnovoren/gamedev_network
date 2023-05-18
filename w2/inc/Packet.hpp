#pragma once

#include "enet/enet.h"
#include <cstring>
#include <cassert>

using byte_t = enet_uint8;

enum class PacketType {
  SERVER_PORT,
  SESSION_START
};

struct Packet {
  Packet(ENetPacket* enet_packet);
  Packet(PacketType in_type);
  Packet(PacketType in_type, byte_t* data, size_t data_size);

  byte_t* GetData();
  const byte_t* GetData() const;

  PacketType type;
  ENetPacket* enet_packet = nullptr;
};
