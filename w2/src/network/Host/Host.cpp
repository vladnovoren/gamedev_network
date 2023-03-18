#include "Host/Host.hpp"
#include "Host/Events.hpp"

Host::Host(int max_n_peers, int max_n_channels) {
  enet_host_ = enet_host_create(nullptr, max_n_peers, max_n_channels, 0, 0);
  if (enet_host_ == nullptr) {
    std::cout << ENET_HOST_CREATION_BAG_MSG << '\n';
  }
}

Host::Host(LocalhostTagT, int port, int max_n_peers, int max_n_channels) {
  ENetAddress address;
  enet_address_set_host(&address, "localhost");
  address.port = port;

  enet_host_ = enet_host_create(&address, max_n_peers, max_n_channels, 0, 0);
  if (enet_host_ == nullptr) {
    std::cout << ENET_HOST_CREATION_BAG_MSG << '\n';
  }
}

Host::Host(int host, int port, int max_n_peers, int max_n_channels) {
  ENetAddress address{.host = (enet_uint32) host, .port = (enet_uint16) port};
  enet_host_ = enet_host_create(&address, max_n_peers, max_n_channels, 0, 0);
  if (enet_host_ == nullptr) {
    std::cout << ENET_HOST_CREATION_BAG_MSG << '\n';
  }
}

std::optional<Peer> Host::Connect(int host, int port, int n_channels) {
  ENetAddress address{.host = (enet_uint32) host, .port = (enet_uint16) port};
  auto enet_peer = enet_host_connect(enet_host_, &address, n_channels, 0);
  if (enet_peer == nullptr) {
    std::cout << ENET_HOST_CONNECTION_BAG_MSG << '\n';
    return {};
  }
  return Peer(enet_peer);
}

std::optional<Peer> Host::Connect(LocalhostTagT, int port, int n_channels) {
  ENetAddress address{.host = 0, .port = (enet_uint16) port};
  enet_address_set_host(&address, "localhost");
  ENetPeer* enet_peer = enet_host_connect(enet_host_, &address, n_channels, 0);
  if (enet_peer == nullptr) {
    std::cout << ENET_HOST_CONNECTION_BAG_MSG << '\n';
    return {};
  }
  return Peer(enet_peer);
}

std::optional<std::unique_ptr<Host::BaseEvent>>
Host::PollEvent(int timeout) {
  ENetEvent enet_event;
  int res = enet_host_service(enet_host_, &enet_event, timeout);
  if (res < 0) {
    std::cout << "error occurred polling event\n";
    return nullptr;
  }
  if (res == 0)
    return {};
  return ConvertEvent(&enet_event);
}

std::unique_ptr<Host::BaseEvent> Host::ConvertEvent(ENetEvent* enet_event) {
  if (enet_event == nullptr)
    return nullptr;
  switch (enet_event->type) {
    case ENET_EVENT_TYPE_NONE:
      return std::unique_ptr<BaseEvent>(new NoneEvent());
    case ENET_EVENT_TYPE_CONNECT:
      return std::unique_ptr<BaseEvent>(
        new ConnectEvent(Peer(enet_event->peer)));
    case ENET_EVENT_TYPE_DISCONNECT:
      return std::unique_ptr<BaseEvent>(
        new DisconnectEvent(Peer(enet_event->peer)));
    case ENET_EVENT_TYPE_RECEIVE:
      return std::unique_ptr<BaseEvent>(
        new ReceiveEvent(Peer(enet_event->peer), enet_event->channelID,
                         Packet((const char*) enet_event->packet->data,
                                enet_event->packet->dataLength)));
  }
  return {};
}

bool Host::IsSameAddress(const NonEmptyEvent& event, const Peer& original) {
  if (event.sender_.GetHost() != original.GetHost() ||
      event.sender_.GetPort() != original.GetPort())
    return false;
  return true;
}

