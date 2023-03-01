#include "DgramSocket.hpp"

DgramSocket::~DgramSocket() = default;

DgramSenderSocket::DgramSenderSocket(const std::string& address, const std::string& port) {
  fd_ = create_dgram_sender_socket(address.c_str(), port.c_str(), &receiver_addr_info_);
}

DgramSenderSocket::~DgramSenderSocket() = default;

ssize_t DgramSenderSocket::SendTo(const char* data, const size_t size) {
  return sendto(fd_, data, size, 0, receiver_addr_info_.ai_addr, receiver_addr_info_.ai_addrlen);
}

DgramReceiverSocket::DgramReceiverSocket(const std::string& port) {
  fd_ = create_dgram_receiver_socket(port.c_str());
}

ssize_t DgramReceiverSocket::RecvFrom(char* buffer, size_t max_size) {
  return recvfrom(fd_, buffer, max_size - 1, 0, nullptr, nullptr);
}

DgramReceiverSocket::~DgramReceiverSocket() = default;

addrinfo DgramSenderSocket::GetReceiverAddrInfo() const {
  return receiver_addr_info_;
}

