#pragma once

#include <vector>
#include "Packet/Packet.hpp"
#include "Host/Host.hpp"
#include "Host/BaseEventHandler.hpp"
#include "Packet/PacketData.hpp"
#include "Tools/AutoArray.hpp"
#include <functional>

class Client {
 public:
  Client();

  ~Client() = default;

  int Run();

 private:
  enum class State {
    GAME_NOT_STARTED,
    GAME_STARTED,
    CONNECTED_TO_GAME_SERVER
  };

  State state_ = State::GAME_NOT_STARTED;

 private:
  std::optional<Peer> ConnectToLobbyServer();

  int LobbyServerProcessor();

  int LobbyRegistry();

  int LobbyStartGameStage();

  int LobbyWaitForStart();

  int LobbyStartGame();

 private:
  using HandlerID = TypeID<Host::BaseEvent>;
  using HandlerT = std::function<int(Client, const Host::BaseEvent&)>;

  void InitEventHandlers();

  int NoneEventHandler(const Host::BaseEvent&);

  int LobbyConnectEventHandler(const Host::BaseEvent& raw_event);

  int LobbyDisconnectEventHandler(const Host::BaseEvent& raw_event);

  int LobbyReceiveEventHandler(const Host::BaseEvent& raw_event);

 private:
  using ProcessorID = TypeID<Packet::BaseData>;
  using ProcessorT = std::function<int(Client, const Packet::BaseData&)>;

  void InitPacketProcessors();

  int GameNotStartedPacketProcessor(const Packet::BaseData&);

  int GameStartedPacketProcessor(const Packet::BaseData&);

  int
  GameServerAddressPacketProcessor(const Packet::BaseData& raw_data);

 private:
  std::string username_;

  Host host_;

  Peer lobby_peer_;
  Peer game_server_peer_;

 private:
  AutoArray<HandlerT> start_game_stage_handlers_;

  AutoArray<ProcessorT> start_game_stage_processors_;
};
