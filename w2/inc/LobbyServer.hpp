#pragma once

#include <vector>
#include "Packet/Packet.hpp"

class LobbyServer {
 public:
  void Run();

 private:
  class IncomingPacketProcessor : public Packet::BaseData::Visitor {
   public:
    explicit IncomingPacketProcessor(LobbyServer& server);

    ~IncomingPacketProcessor() override = default;

    void Visit(Packet::RegistryData& registry_data) override;

//    void Visit(Packet::MsgData& msg_data) override;

   private:
    LobbyServer& server_;
  };

 private:
  IncomingPacketProcessor incoming_packet_processor_;

//  std::vector<>
};