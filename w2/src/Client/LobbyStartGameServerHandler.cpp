#include "Client/Client.hpp"

void Client::LobbyStartGameEventHandler::Handle(
  const Host::ReceiveEvent& event) {
  auto event_data = event.packet_.ExtractData();
  event_data->Accept(packet_receiver_);
}

void Client::LobbyStartGameEventHandler::IncomingPacketReceiver::Visit(
  Packet::GameNotStartedData& data) {
}

void Client::LobbyStartGameEventHandler::IncomingPacketReceiver::Visit(
  Packet::GameStartedData& data) {
}

void Client::LobbyStartGameEventHandler::IncomingPacketReceiver::Visit(
  Packet::GameServerAddressData& data) {
}
