#pragma once

#include "DgramSocket.hpp"
#include <utility>

class Client {
 public:
  Client(const std::string& address, const std::string& to_server_port,
         const std::string& to_client_port);

  ~Client() = default;

  void Run();
 private:
  class IncomingPackageProcessor : public Package::BaseData::Visitor {
   public:
    explicit IncomingPackageProcessor(Client& client);

    ~IncomingPackageProcessor() override = default;

    void Visit(Package::MsgData& msg_data) override;

   private:
    Client& client_;
  };

 private:
  void Register();

  void Queries();

  void KeepAlive();

 private:
  std::string username_;

  DgramSenderSocket sender_socket_;
  DgramReceiverSocket receiver_socket_;

  IncomingPackageProcessor incoming_package_procesoor_;
};