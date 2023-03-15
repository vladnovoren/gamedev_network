#include "Client/Client.hpp"
#include "Client/LobbyStartGameEventHandler.hpp"
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
}

int Client::Run() {
  auto lobby_peer = ConnectToLobbyServer();
  if (!lobby_peer.has_value())
    return -1;
  return 0;
}

std::optional<Peer> Client::ConnectToLobbyServer() {
  return host_.Connect(LocalhostTag, LOBBY_SERVER_PORT, 2);
}

std::optional<Peer> Client::ConnectToGameServer() {
  return host_.Connect(LocalhostTag, GAME_SERVER_PORT, 2);
}

void Client::LobbyServerProcessor(Peer& lobby_server_peer) {
  LobbyRegistry(lobby_server_peer);
}

void Client::LobbyRegistry(Peer& lobby_server_peer) {
  std::cout << "Enter username: ";
  std::cin >> username_;

  auto registry_packet = Packet::RegistryData(username_).MakePacket();
  lobby_server_peer.Send(registry_packet);
}

void Client::LobbyStartGameEventHandler::Handle(
  const Host::ReceiveEvent& event) {
  auto packet_data = event.packet_.ExtractData();
}

void Client::LobbyStartGame() {
  std::optional<std::unique_ptr<Host::BaseEvent>> event_ptr;
  while (LobbyStartGameLoopCriteria()) {
    event_ptr = host_.PollEvent(10);
    event_ptr.value()->Handle(lobby_start_game_event_handler_);

//    lobby_start_game_event_handler_.Handle(*event_ptr.value());
  }
}
