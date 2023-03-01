#pragma once

#include <string>
#include <sys/socket.h>
#include <netdb.h>
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

  ssize_t SendTo(const char* data, size_t size);

 private:
  addrinfo receiver_addr_info_;
  bool connected_ = false;
};

class DgramReceiverSocket : public DgramSocket {
 public:
  DgramReceiverSocket(const std::string& port);
  ~DgramReceiverSocket() override;

  ssize_t RecvFrom(char* buffer, size_t max_size);
};