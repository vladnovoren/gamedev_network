#pragma once
#include <enet/enet.h>
#include <cstdint>
#include "entity.h"
#include "quantisation.h"
#include "BitStream.hpp"

void send_join(ENetPeer *peer);
void send_new_entity(ENetPeer *peer, const Entity &ent);
void send_set_controlled_entity(ENetPeer *peer, uint16_t eid);
void send_entity_input(ENetPeer *peer, uint16_t eid, float thr, float steer);
void send_snapshot(ENetPeer *peer, uint16_t eid, float x, float y, float ori);

MessageType get_packet_type(ENetPacket *packet);

void deserialize_new_entity(ENetPacket *packet, Entity &ent);
void deserialize_set_controlled_entity(ENetPacket *packet, uint16_t &eid);
void deserialize_entity_input(ENetPacket *packet, uint16_t &eid, float &thr, float &steer);
void deserialize_snapshot(ENetPacket *packet, uint16_t &eid, float &x, float &y, float &ori);

