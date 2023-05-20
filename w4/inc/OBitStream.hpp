#pragma once

#include "Core.hpp"
#include <cstring>

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

 private:
  const byte_t* data_ = nullptr;
  size_t offset_ = 0;
};
