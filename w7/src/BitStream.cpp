#include "BitStream.hpp"

OBitStream::OBitStream(ENetPacket* packet) : data_(packet->data) {
  offset_ += sizeof(MessageType);
}

OBitStream::OBitStream(const byte_t* data) : data_(data) {
}

void OBitStream::ReadPackedUInt32(uint32_t& value) {
  uint32_t res = *(uint32_t*)(data_);
  if ((res & 0x1) == 0) {
    offset_ += sizeof(uint8_t);
    value = (res & 0xFF) >> 1;
  } else if ((res & 0x3) == 1) {
    offset_ += sizeof(uint16_t);
    value = (res & 0xFFFF) >> 2;
  } else {
    offset_ += sizeof(uint32_t);
    value = (res) >> 2;
  }
}

IBitStream::IBitStream(ENetPacket* packet) : data_(packet->data) {
}

IBitStream::IBitStream(byte_t* data) : data_(data) {
}

void IBitStream::WritePackedUInt32(uint32_t value) {
  if (value <= 0x7F) {
    offset_ += sizeof(uint8_t);
    uint8_t* data = (uint8_t*)(data_);
    *data = (value << 1) & 0xFE;
  } else if (value <= 0x3FFF) {
    offset_ += sizeof(uint16_t);
    uint16_t* data = (uint16_t*)(data_);
    *data = (value << 2) & 0xFFFC | 0x1;
  } else if (value <= 0x3FFF'FFFF) {
    offset_ += sizeof(uint32_t);
    uint32_t* data = (uint32_t*)(data_);
    *data = (value << 2) & 0xFFFF'FFFC | 0x3;
  }
}
