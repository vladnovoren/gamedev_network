#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

using byte = uint8_t;

class BitStream {
public:
  explicit BitStream(byte *buffer, size_t capacity);

  template <typename T> void Write(const T &value) {
    assert(size_ + sizeof(T) <= capacity_);

    size_ += sizeof(T);
    memcpy(buffer_, &value, sizeof(T));
    buffer_ += sizeof(T);
  }

  template <typename T> void Read(T &value) {
    assert(size_ + sizeof(T) <= capacity_);

    size_ += sizeof(T);
    memcpy(&value, buffer_, sizeof(T));
    buffer_ += sizeof(T);
  }

private:
  byte *buffer_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};