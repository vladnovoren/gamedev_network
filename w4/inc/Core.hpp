#pragma once

#include <enet/enet.h>

enum MessageType : uint8_t
{
  E_CLIENT_TO_SERVER_JOIN = 0,
  E_SERVER_TO_CLIENT_NEW_ENTITY,
  E_SERVER_TO_CLIENT_SET_CONTROLLED_ENTITY,
  E_CLIENT_TO_SERVER_STATE,
  E_SERVER_TO_CLIENT_SNAPSHOT
};


using byte_t = uint8_t;
using port_t = uint16_t;
