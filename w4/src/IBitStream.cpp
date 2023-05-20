#include "IBitStream.hpp"

IBitStream::IBitStream(ENetPacket* packet) : data_(packet->data) {
}

IBitStream::IBitStream(byte_t* data) : data_(data) {
}

