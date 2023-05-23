#pragma once

#include "enet/enet.h"
#include <cstring>

enum MessageType : uint8_t
{
  E_CLIENT_TO_SERVER_JOIN = 0,
  E_SERVER_TO_CLIENT_NEW_ENTITY,
  E_SERVER_TO_CLIENT_SET_CONTROLLED_ENTITY,
  E_CLIENT_TO_SERVER_INPUT,
  E_SERVER_TO_CLIENT_SNAPSHOT
};

using byte_t = uint8_t;

class OBitStream {
 public:
  OBitStream(ENetPacket* packet);
  OBitStream(const byte_t* data);

  template<typename T>
  void Read(T& val) {
    memcpy(&val, data_ + offset_, sizeof(T));
    offset_ += sizeof(T);
  }

  template<typename T>
  void Skip() {
    offset_ += sizeof(T);
  }

  void ReadPackedUInt32(uint32_t& value);

 private:
  const byte_t* data_ = nullptr;
  size_t offset_ = 0;
};

class IBitStream {
 public:
  IBitStream(ENetPacket* packet);
  IBitStream(byte_t* data);

  template<typename T>
  void Write(const T& val) {
    memcpy(data_ + offset_, &val, sizeof(T));
    offset_ += sizeof(T);
  }

  void WritePackedUInt32(uint32_t value);

 private:
  byte_t* data_ = nullptr;
  size_t offset_ = 0;
};
