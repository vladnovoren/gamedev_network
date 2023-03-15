#include "Peer.hpp"
#include "Host/Host.hpp"

int main() {
  Host host(1, 1);
  auto server_peer = host.Connect(LocalhostTag, 2023, 1);
  if (!server_peer.has_value()) {
    std::cout << "can't connect\n";
  }
  return 0;
}