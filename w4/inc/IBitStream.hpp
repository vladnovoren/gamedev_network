#pragma once

#include "Core.hpp"
#include <cstring>

class IBitStream {
 public:
  IBitStream(ENetPacket* packet);
  IBitStream(byte_t* data);

  template<typename T>
  void Write(const T& val) {
    memcpy(data_ + offset_, &val, sizeof(T));
    offset_ += sizeof(T);
  }

 private:
  byte_t* data_ = nullptr;
  size_t offset_ = 0;
};
