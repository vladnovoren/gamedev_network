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
  i_bit_stream.WritePackedUInt32(eid);
  enet_peer_send(peer, 0, packet);
}

void send_entity_input(ENetPeer *peer, uint16_t eid, float thr, float ori) {
  ENetPacket *packet = enet_packet_create(nullptr, sizeof(uint8_t) +
                                                   sizeof(uint16_t) +
                                                   sizeof(uint8_t),
                                          ENET_PACKET_FLAG_UNSEQUENCED);

  float4bitsQuantized packed_thr(thr, -1.f, 1.f);
  float4bitsQuantized packed_ori(ori, -1.f, 1.f);
  uint8_t packed_thr_steer = (packed_thr.packed_val << 4) |
                             packed_ori.packed_val;

  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_CLIENT_TO_SERVER_INPUT);
  i_bit_stream.WritePackedUInt32(eid);
  i_bit_stream.Write(packed_thr_steer);
  enet_peer_send(peer, 1, packet);
}

void send_snapshot(ENetPeer *peer, uint16_t eid, float x, float y,
                   float ori) {
  ENetPacket *packet = enet_packet_create(nullptr, sizeof(uint8_t) +
                                                   sizeof(uint16_t) +
                                                   sizeof(uint16_t) +
                                                   sizeof(uint16_t) +
                                                   sizeof(uint8_t),
                                          ENET_PACKET_FLAG_UNSEQUENCED);

  PositionXQuantized packed_x(x, -16, 16);
  PositionYQuantized packed_y(y, -8, 8);

  uint8_t oriPacked = pack_float<uint8_t>(ori, -PI, PI, 8);

  IBitStream i_bit_stream(packet);
  i_bit_stream.Write(E_SERVER_TO_CLIENT_SNAPSHOT);
  i_bit_stream.WritePackedUInt32(eid);
  i_bit_stream.Write(packed_x.packed_val);
  i_bit_stream.Write(packed_y.packed_val);
  i_bit_stream.Write(oriPacked);

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
  uint32_t tmp;
  o_bit_stream.ReadPackedUInt32(tmp);
  eid = tmp;
}

void deserialize_entity_input(ENetPacket *packet, uint16_t &eid,
                              float &thr, float &steer) {
  OBitStream o_bit_stream(packet);

  uint32_t tmp;
  o_bit_stream.ReadPackedUInt32(tmp);
  eid = tmp;

  uint8_t packed_thr_steer;
  o_bit_stream.Read(packed_thr_steer);

  static uint8_t neutral_packed_value = pack_float<uint8_t>(0.f, -1.f, 1.f, 4);
  static uint8_t nominal_packed_value = pack_float<uint8_t>(1.f, 0.f, 1.2f, 4);

  float4bitsQuantized packed_thr(packed_thr_steer >> 4);
  float4bitsQuantized packed_steer(packed_thr_steer & 0x0f);

  float unpacked_thr = packed_thr.unpack(-1.f, 1.f);
  float unpacked_steer = packed_steer.unpack(-1.f, 1.f);

  thr = packed_thr.packed_val == neutral_packed_value ? 0.f : unpacked_thr;
  steer =
    packed_steer.packed_val == neutral_packed_value ? 0.f : unpacked_steer;
}

void deserialize_snapshot(ENetPacket *packet, uint16_t &eid,
                          float &x, float &y, float &ori) {
  OBitStream o_bit_stream(packet);

  uint32_t tmp;
  o_bit_stream.ReadPackedUInt32(tmp);
  eid = tmp;

  uint16_t packed_x;
  uint16_t packed_y;
  uint8_t packed_ori;

  o_bit_stream.Read(packed_x);
  o_bit_stream.Read(packed_y);
  o_bit_stream.Read(packed_ori);

  PositionXQuantized packed_x_val(packed_x);
  PositionYQuantized packed_y_val(packed_y);

  x = packed_x_val.unpack(-16, 16);
  y = packed_y_val.unpack(-8, 8);
  ori = unpack_float<uint8_t>(packed_ori, -PI, PI, 8);
}
