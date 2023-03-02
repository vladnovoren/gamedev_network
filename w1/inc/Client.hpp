#pragma once

#include "DgramSocket.hpp"
#include <utility>

class Client {
 public:
  Client(std::string username, const std::string& address,
         const std::string& to_server_port, const std::string& to_client_port);
  ~Client() = default;

  void Run();

 private:
  void Register();

 private:
  const std::string username_;

  DgramSenderSocket sender_socket_;
  DgramReceiverSocket receiver_socket_;
};