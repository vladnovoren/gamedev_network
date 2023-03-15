#pragma once

#include <vector>
#include "Packet/Packet.hpp"
#include "Host/Host.hpp"
#include "Host/BaseEventHandler.hpp"
#include "Packet/PacketData.hpp"
#include "LobbyStartGameEventHandler.hpp"

class Client {
 public:
  Client();

  ~Client() = default;

  int Run();

 private:
  std::optional<Peer> ConnectToLobbyServer();
  std::optional<Peer> ConnectToGameServer();

  void LobbyServerProcessor(Peer& lobby_server_peer);

  void LobbyRegistry(Peer& lobby_server_peer);
  void LobbyStartGame();

  bool LobbyStartGameLoopCriteria();

  void Queries();

 private:
  class LobbyStartGameEventHandler : public Host::BaseEventHandler {
   public:
    LobbyStartGameEventHandler();

    void Handle(const Host::ReceiveEvent& event) override;

    void Handle(const Host::)

    class IncomingPacketReceiver : public Packet::BaseData::Visitor {
     public:
      void Visit(Packet::GameNotStartedData& data) override;

      void Visit(Packet::GameStartedData& data) override;

      void Visit(Packet::GameServerAddressData& data) override;
    };

   private:
    IncomingPacketReceiver packet_receiver_;
  };

 private:
  LobbyStartGameEventHandler lobby_start_game_event_handler_;
  std::string username_;

  Host host_;
};

