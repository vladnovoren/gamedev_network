#pragma once

#include "enet/enet.h"
#include <cstring>
#include <cassert>
#include <string>

using byte_t = enet_uint8;

enum class PacketType {
  SERVER_PORT,
  SESSION_START,
  MESSAGE
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

void SendMessage(ENetPeer* peer, const std::string& msg);
