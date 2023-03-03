#include "DgramSocket.hpp"

DgramSocket::~DgramSocket() = default;

DgramSenderSocket::DgramSenderSocket(const std::string& address,
                                     const std::string& port) {
  fd_ = create_dgram_sender_socket(address.c_str(), port.c_str(),
                                   &receiver_addr_info_);
}

DgramSenderSocket::~DgramSenderSocket() = default;

ssize_t DgramSenderSocket::Send(const Packet& packet) {
  return sendto(fd_, packet.Mem(), packet.MemSize(), 0,
                receiver_addr_info_.ai_addr, receiver_addr_info_.ai_addrlen);
}

DgramReceiverSocket::DgramReceiverSocket(const std::string& port) {
  fd_ = create_dgram_receiver_socket(port.c_str());
}

std::optional<Packet> DgramReceiverSocket::Receive() {
  Packet packet;

  fd_set read_set;
  FD_ZERO(&read_set);
  FD_SET(fd_, &read_set);

  timeval timeout = {0, 100000};
  select(fd_ + 1, &read_set, nullptr, nullptr, &timeout);

  if (FD_ISSET(fd_, &read_set)) {
    ssize_t recv_res = recvfrom(fd_, packet.Mem(), Packet::MAX_MEM_SIZE, 0,
                                nullptr,
                                nullptr);
    if (recv_res < 0) {
      std::cout << "bug\n";
      std::cout << strerror(errno) << '\n';
      std::cout << "file descriptor: " << fd_ << '\n';
      return {};
    }
    if (recv_res == 0) {
      std::cout << "nothing\n";
      return {};
    }
    return packet;
  }

  return {};
}

DgramReceiverSocket::~DgramReceiverSocket() = default;

addrinfo DgramSenderSocket::GetReceiverAddrInfo() const {
  return receiver_addr_info_;
}
