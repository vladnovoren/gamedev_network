#include "Client/Client.hpp"
#include "Host/Events.hpp"


//void send_fragmented_packet(ENetPeer* peer) {
//  const char* baseMsg = "Stay awhile and listen. ";
//  const size_t msgLen = strlen(baseMsg);
//
//  const size_t sendSize = 2500;
//  char* hugeMessage = new char[sendSize];
//  for (size_t i = 0; i < sendSize; ++i)
//    hugeMessage[i] = baseMsg[i % msgLen];
//  hugeMessage[sendSize - 1] = '\0';
//
//  ENetPacket* packet = enet_packet_create(hugeMessage, sendSize,
//                                          ENET_PACKET_FLAG_RELIABLE);
//  enet_peer_send(peer, 0, packet);
//
//  delete[] hugeMessage;
//}
//
//void send_micro_packet(ENetPeer* peer) {
//  const char* msg = "dv/dt";
//  ENetPacket* packet = enet_packet_create(msg, strlen(msg) + 1,
//                                          ENET_PACKET_FLAG_UNSEQUENCED);
//  enet_peer_send(peer, 1, packet);
//}
//
//int main(int argc, const char** argv) {
//  if (enet_initialize() != 0) {
//    printf("Cannot init ENet");
//    return 1;
//  }
//
//  ENetHost* Client = enet_host_create(nullptr, 1, 2, 0, 0);
//  if (!Client) {
//    printf("Cannot create ENet Client\n");
//    return 1;
//  }
//
//  ENetAddress address;
//  enet_address_set_host(&address, "localhost");
//  address.port = 10887;
//
//  ENetPeer* lobbyPeer = enet_host_connect(Client, &address, 2, 0);
//  if (!lobbyPeer) {
//    printf("Cannot connect to lobby");
//    return 1;
//  }
//
//  uint32_t timeStart = enet_time_get();
//  uint32_t lastFragmentedSendTime = timeStart;
//  uint32_t lastMicroSendTime = timeStart;
//  bool connected = false;
//  while (true) {
//    ENetEvent event;
//    while (enet_host_service(Client, &event, 10) > 0) {
//      switch (event.type) {
//        case ENET_EVENT_TYPE_CONNECT:
//          printf("Connection with %x:%u established\n",
//                 event.peer->address.host, event.peer->address.port);
//          connected = true;
//          break;
//        case ENET_EVENT_TYPE_RECEIVE:
//          printf("Packet received '%s'\n", event.packet->data);
//          enet_packet_destroy(event.packet);
//          break;
//        default:
//          break;
//      };
//    }
//    if (connected) {
//      uint32_t curTime = enet_time_get();
//      if (curTime - lastFragmentedSendTime > 1000) {
//        lastFragmentedSendTime = curTime;
//        send_fragmented_packet(lobbyPeer);
//      }
//      if (curTime - lastMicroSendTime > 100) {
//        lastMicroSendTime = curTime;
//        send_micro_packet(lobbyPeer);
//      }
//    }
//  }
//  return 0;
//}

Client::Client() : host_(2, 4) {
  InitEventHandlers();
}

int Client::Run() {
  auto opt_lobby_peer = ConnectToLobbyServer();
  if (!opt_lobby_peer.has_value())
    return -1;
  lobby_peer_ = std::move(opt_lobby_peer.value());
  return 0;
}

std::optional<Peer> Client::ConnectToLobbyServer() {
  return host_.Connect(LocalhostTag, LOBBY_SERVER_PORT, 2);
}

int Client::LobbyServerProcessor() {
  if (LobbyRegistry() < 0)
    return -1;
  if (LobbyStartGameStage() < 0)
    return -1;
  return 0;
}

int Client::LobbyRegistry() {
  std::cout << "Enter username: ";
  std::cin >> username_;

  auto registry_packet = Packet::RegistryData(username_).MakePacket();
  return lobby_peer_.Send(registry_packet);
}

int Client::LobbyStartGameStage() {
  std::optional<std::unique_ptr<Host::BaseEvent>> opt_event_ptr;
  while (state_ != State::CONNECTED_TO_GAME_SERVER) {
    opt_event_ptr = host_.PollEvent(10);
    if (!opt_event_ptr.has_value())
      continue;
    if (opt_event_ptr.value() == nullptr)
      return -1;
    Host::BaseEvent& event = *opt_event_ptr.value();
    if (start_game_stage_handlers_[event.GetID()](*this, event) < 0)
      return -1;
  }
}

int Client::LobbyWaitForStart() {
  std::string input_str;
  while (std::getline(std::cin, input_str)) {
    if (input_str == "start") {
      return LobbyStartGame();
    } else {
      std::cout << "incorrect query, to start game type \"start\"\n";
    }
  }
  return 0;
}

int Client::LobbyStartGame() {
  return lobby_peer_.Send(Packet::StartGameData().MakePacket());
}

void Client::InitEventHandlers() {
  start_game_stage_handlers_[HandlerID::GetID<Host::NoneEvent>()] = &Client::NoneEventHandler;
  start_game_stage_handlers_[HandlerID::GetID<Host::ConnectEvent>()] = &Client::LobbyConnectEventHandler;
  start_game_stage_handlers_[HandlerID::GetID<Host::DisconnectEvent>()] = &Client::LobbyDisconnectEventHandler;
  start_game_stage_handlers_[HandlerID::GetID<Host::ReceiveEvent>()] = &Client::LobbyReceiveEventHandler;
}

int Client::NoneEventHandler(const Host::BaseEvent&) {
  return 0;
}

int Client::LobbyConnectEventHandler(const Host::BaseEvent& raw_event) {
  auto event = static_cast<const Host::ConnectEvent&>(raw_event);
  if (!Host::IsSameAddress(event, lobby_peer_)) {
    std::cout << "unknown connection, lobby server required\n";
    return -1;
  }
  std::cout << "connection with lobby server established\n";
  return 0;
}

int Client::LobbyDisconnectEventHandler(const Host::BaseEvent&) {
  return 0;
}

int Client::LobbyReceiveEventHandler(const Host::BaseEvent& raw_event) {
  auto event = static_cast<const Host::ReceiveEvent&>(raw_event);
  if (!Host::IsSameAddress(event, lobby_peer_)) {
    std::cout << "packet received from unknown host\n";
    return -1;
  }

  auto data_ptr = event.packet.ExtractData();
  if (data_ptr == nullptr)
    return -1;

  return start_game_stage_processors_[data_ptr->GetID()](*this, *data_ptr);
}

void Client::InitPacketProcessors() {
  start_game_stage_processors_[ProcessorID::GetID<Packet::GameNotStartedData>()] = &Client::GameNotStartedPacketProcessor;
  start_game_stage_processors_[ProcessorID::GetID<Packet::GameStartedData>()] = &Client::GameStartedPacketProcessor;
  start_game_stage_processors_[ProcessorID::GetID<Packet::GameServerAddressData>()] = &Client::GameServerAddressPacketProcessor;
}

int Client::GameNotStartedPacketProcessor(const Packet::BaseData&) {
  return LobbyWaitForStart();
}

int Client::GameStartedPacketProcessor(const Packet::BaseData&) {
  state_ = State::GAME_STARTED;
  return 0;
}

int Client::GameServerAddressPacketProcessor(const Packet::BaseData& raw_data) {
  auto address_data = static_cast<const Packet::GameServerAddressData&>(raw_data);
  auto opt_game_server_peer_ = host_.Connect(address_data.host,
                                             address_data.port, 1);
  if (!opt_game_server_peer_.has_value()) {
    std::cout << "error connecting to game server\n";
    return -1;
  }
  game_server_peer_ = std::move(opt_game_server_peer_.value());
  state_ = State::CONNECTED_TO_GAME_SERVER;
  return 0;
}