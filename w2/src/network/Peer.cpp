#include "Peer.hpp"

Peer::Peer(ENetPeer* enet_peer) : enet_peer_(enet_peer) {
}

int Peer::Send(const Packet& packet) {
  auto enet_packet = enet_packet_create(packet.Mem(), packet.MemSize(), 0);
  if (enet_packet == nullptr) {
    std::cout << ENET_PACKET_CREATION_BAG_NSG << '\n';
    return -1;
  }
  if (enet_peer_send(enet_peer_, 0, enet_packet) < 0) {
    std::cout << ENET_PACKET_SEND_BAG_MSG << '\n';
    return -1;
  }
  return 0;
}

int Peer::GetHost() const {
  assert(enet_peer_ != nullptr);
  return (int) enet_peer_->address.host;
}

int Peer::GetPort() const {
  assert(enet_peer_ != nullptr);
  return enet_peer_->address.port;
}

