#include "Packet.hpp"

Packet::Packet(ENetPacket* in_enet_packet) {
  assert(in_enet_packet != nullptr);

  type = *(PacketType*)in_enet_packet->data;
  enet_packet = in_enet_packet;
}

Packet::Packet(PacketType in_type, byte_t* data, size_t data_size)
    : type(in_type) {
  byte_t* data_with_type = (byte_t*)malloc(sizeof(PacketType) + data_size);
  memcpy(data_with_type, &in_type, sizeof(PacketType));
  memcpy(data_with_type + sizeof(PacketType), data, data_size);
  enet_packet = enet_packet_create(data_with_type, data_size,
                                   ENET_PACKET_FLAG_RELIABLE);
  free(data_with_type);
}

byte_t* Packet::GetData() {
  return enet_packet->data + sizeof(PacketType);
}