#pragma once

#include <string>
#include <optional>
#include <sys/socket.h>
#include <netdb.h>
#include "Package.hpp"
#include "socket_init.hpp"

class DgramSocket {
 public:
  virtual ~DgramSocket() = 0;

 protected:
  int fd_;
};

class DgramSenderSocket : public DgramSocket {
 public:
  DgramSenderSocket(const std::string& address, const std::string& port);
  ~DgramSenderSocket() override;

  [[nodiscard]] addrinfo GetReceiverAddrInfo() const;

  ssize_t Send(const Package& package);

 private:
  addrinfo receiver_addr_info_ = {};
  bool connected_ = false;
};

class DgramReceiverSocket : public DgramSocket {
 public:
  explicit DgramReceiverSocket(const std::string& port);
  ~DgramReceiverSocket() override;

  std::optional<Package> Receive();
};