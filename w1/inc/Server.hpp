#pragma once

#include "DgramSocket.hpp"

class Server {
 public:
  Server(const std::string& address, const std::string& port_to_server,
         const std::string& port_to_client);

  void Run();

 private:
  class IncomingPackageProcessor : public Package::BaseData::Visitor {
   public:
    explicit IncomingPackageProcessor(Server& server);

    ~IncomingPackageProcessor() override = default;

    void Visit(Package::RegistryData& registry_data) override;

    void Visit(Package::KeepAliveData& keep_alive_data) override;

    void Visit(Package::MsgData& msg_data) override;

   private:
    Server& server_;
  };

 private:
  DgramSenderSocket sender_socket_;
  DgramReceiverSocket receiver_socket_;

  IncomingPackageProcessor incoming_package_processor_;
};