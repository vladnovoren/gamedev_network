#pragma once

#include <cassert>
#include <cstdio>
#include <iostream>
#include "enet/enet.h"

using host_t = enet_uint32;
using port_t = enet_uint16;

void LogConnectionEstablished(const ENetAddress& address);

void LogDisconnected(const ENetAddress& address);

bool IsEventFromPeer(const ENetEvent& event, const ENetPeer* peer);

struct Address {
  host_t host;
  port_t port;
};
