#include "OBitStream.hpp"

OBitStream::OBitStream(ENetPacket* packet) : data_(packet->data) {
  offset_ += sizeof(MessageType);
}

OBitStream::OBitStream(const byte_t* data) : data_(data) {
}
