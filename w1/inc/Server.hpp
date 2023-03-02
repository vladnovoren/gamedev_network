#pragma once

#include "DgramSocket.hpp"

class Server {
 public:
  Server(const std::string& address, const std::string& port_to_server,
         const std::string& port_to_client);

  void Run();

 private:
  DgramSenderSocket sender_socket_;
  DgramReceiverSocket receiver_socket_;
};