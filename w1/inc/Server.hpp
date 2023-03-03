#pragma once

#include "DgramSocket.hpp"

class Server {
 public:
  Server(const std::string& address, const std::string& port_to_server,
         const std::string& port_to_client);

  void Run();

 private:
  class IncomingPacketProcessor : public Packet::BaseData::Visitor {
   public:
    explicit IncomingPacketProcessor(Server& server);

    ~IncomingPacketProcessor() override = default;

    void Visit(Packet::RegistryData& registry_data) override;

    void Visit(Packet::KeepAliveData& keep_alive_data) override;

    void Visit(Packet::MsgData& msg_data) override;

   private:
    Server& server_;
  };

 private:
  DgramSenderSocket sender_socket_;
  DgramReceiverSocket receiver_socket_;

  IncomingPacketProcessor incoming_packet_processor_;
};