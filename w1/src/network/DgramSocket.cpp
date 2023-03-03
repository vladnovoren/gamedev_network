#include "DgramSocket.hpp"

DgramSocket::~DgramSocket() = default;

DgramSenderSocket::DgramSenderSocket(const std::string& address, const std::string& port) {
  fd_ = create_dgram_sender_socket(address.c_str(), port.c_str(), &receiver_addr_info_);
}

DgramSenderSocket::~DgramSenderSocket() = default;

ssize_t DgramSenderSocket::Send(const Package& package) {
  return sendto(fd_, package.Mem(), package.MemSize(), 0,
                receiver_addr_info_.ai_addr, receiver_addr_info_.ai_addrlen);
}

DgramReceiverSocket::DgramReceiverSocket(const std::string& port) {
  fd_ = create_dgram_receiver_socket(port.c_str());
}

std::optional<Package> DgramReceiverSocket::Receive() {
  Package package;

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd_, &read_set);

  timeval timeout = { 0, 100000 };
  select(fd_ + 1, &read_set, nullptr, nullptr, &timeout);

  if (FD_ISSET(fd_, &read_set)) {
    recvfrom(fd_, package.Mem(), Package::MAX_MEM_SIZE, 0, nullptr, nullptr);
    return package;
  }

  return {};
}

DgramReceiverSocket::~DgramReceiverSocket() = default;

addrinfo DgramSenderSocket::GetReceiverAddrInfo() const {
  return receiver_addr_info_;
}
