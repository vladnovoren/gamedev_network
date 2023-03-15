#pragma once

#include <enet/enet.h>
#include <iostream>
#include <optional>
#include "Peer.hpp"
#include "Packet/Packet.hpp"

struct LocalhostTagT {
};

static constexpr LocalhostTagT LocalhostTag = {};

class Host {
 public:
  class BaseEvent;

  class BaseEventHandler;

 public:
  Host(int max_n_peers, int max_n_channels);

  Host(LocalhostTagT, int port, int max_n_peers, int max_n_channels);

  Host(int host, int port, int max_n_peers, int max_n_channels);

  std::optional<Peer> Connect(int host, int port, int n_channels);

  std::optional<Peer> Connect(LocalhostTagT, int port, int n_channels);

  std::optional<std::unique_ptr<BaseEvent>>
  PollEvent(int timeout);

 public:
  class NoneEvent;

  class NonEmptyEvent;

  class ConnectEvent;

  class DisconnectEvent;

  class ReceiveEvent;

  static std::unique_ptr<BaseEvent> ConvertEvent(ENetEvent* enet_event);

 private:
  static constexpr char const* ENET_HOST_CREATION_BAG_MSG = "cannot create ENetHost";
  static constexpr char const* ENET_HOST_CONNECTION_BAG_MSG = "cannot connect to remote host";
 private:
  ENetHost* enet_host_ = nullptr;
};
