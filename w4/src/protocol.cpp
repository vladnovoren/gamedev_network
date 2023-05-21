#include "protocol.h"

void send_join(ENetPeer* peer) {
  auto packet = enet_packet_create(nullptr, sizeof(MessageType),
                                   ENET_PACKET_FLAG_RELIABLE);
  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_CLIENT_TO_SERVER_JOIN);
  enet_peer_send(peer, 0, packet);
}

void send_new_entity(ENetPeer* peer, const Entity& ent) {
  auto packet = enet_packet_create(nullptr,
                                   sizeof(MessageType) + sizeof(Entity),
                                   ENET_PACKET_FLAG_RELIABLE);
  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_SERVER_TO_CLIENT_NEW_ENTITY);
  i_bit_stream.Write(ent);
  enet_peer_send(peer, 0, packet);
}

void send_set_controlled_entity(ENetPeer* peer, uint16_t eid) {
  auto packet = enet_packet_create(nullptr,
                                   sizeof(MessageType) + sizeof(uint16_t),
                                   ENET_PACKET_FLAG_RELIABLE);
  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_SERVER_TO_CLIENT_SET_CONTROLLED_ENTITY);
  i_bit_stream.Write(eid);
  enet_peer_send(peer, 0, packet);
}

void send_entity_state(ENetPeer* peer, uint16_t eid, float x, float y) {
  auto packet = enet_packet_create(nullptr,
                                   sizeof(MessageType) + sizeof(uint16_t) +
                                   2 * sizeof(float),
                                   ENET_PACKET_FLAG_UNSEQUENCED);
  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_CLIENT_TO_SERVER_STATE);
  i_bit_stream.Write(eid);
  i_bit_stream.Write(x);
  i_bit_stream.Write(y);
  enet_peer_send(peer, 1, packet);
}

void send_snapshot(ENetPeer* peer, uint16_t eid, float x, float y,
                   float radius) {
  auto packet = enet_packet_create(nullptr, sizeof(MessageType) +
                                            sizeof(uint16_t) +
                                            2 * sizeof(float) + sizeof(float),
                                            ENET_PACKET_FLAG_UNSEQUENCED);
  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_SERVER_TO_CLIENT_SNAPSHOT);
  i_bit_stream.Write(eid);
  i_bit_stream.Write(x);
  i_bit_stream.Write(y);
  i_bit_stream.Write(radius);
  enet_peer_send(peer, 1, packet);
}

MessageType get_packet_type(ENetPacket* packet) {
  return (MessageType)*packet->data;
}

void deserialize_new_entity(ENetPacket* packet, Entity& ent) {
  OBitStream o_bit_stream(packet);
  o_bit_stream.Read(ent);
}

void deserialize_set_controlled_entity(ENetPacket* packet, uint16_t& eid) {
  OBitStream o_bit_stream(packet);
  o_bit_stream.Read(eid);
}

void deserialize_entity_state(ENetPacket* packet, uint16_t &eid,
                              float &x, float &y) {
  OBitStream o_bit_stream(packet);
  o_bit_stream.Read(eid);
  o_bit_stream.Read(x);
  o_bit_stream.Read(y);
}

void deserialize_snapshot(ENetPacket* packet, uint16_t& eid,
                          float& x, float& y, float& radius) {
  OBitStream o_bit_stream(packet);
  o_bit_stream.Read(eid);
  o_bit_stream.Read(x);
  o_bit_stream.Read(y);
  o_bit_stream.Read(radius);
}

void LogConnectionEstablished(const ENetAddress& address) {
  printf("Connection established with: [host = %d, port = %d]\n",
         address.host, address.port);
}

void LogDisconnected(const ENetAddress& address) {
  printf("Disconnected from: [host = %d, port = %d]\n",
         address.host, address.port);
}
