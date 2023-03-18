#include "Packet/PacketData.hpp"

Packet Packet::BaseData::MakePacket() {
  return {};
}

int Packet::BaseData::GetID() const {
  return TypeID<BaseT>::GetID<BaseData>();
}

Packet::RegistryData::RegistryData(std::string username) :
  username_(std::move(username)) {
}

const std::string& Packet::RegistryData::GetUsername() const {
  return username_;
}

Packet Packet::RegistryData::MakePacket() {
  Packet raw_packet(PacketType::REGISTRY);
  raw_packet.Append(username_.c_str(), username_.size());
  return raw_packet;
}

int Packet::RegistryData::GetID() const {
  return TypeID<BaseT>::GetID<RegistryData>();
}

Packet::MsgData::MsgData(std::string msg) :
  msg_(std::move(msg)) {
  assert(msg_.size() <= MAX_MSG_LEN);
}

Packet Packet::MsgData::MakePacket() {
  Packet raw_packet(PacketType::MSG);
  raw_packet.Append(msg_.c_str(), msg_.size());
  return raw_packet;
}

const std::string& Packet::MsgData::GetMsg() const {
  return msg_;
}

int Packet::MsgData::GetID() const {
  return TypeID<BaseT>::GetID<MsgData>();
}

Packet Packet::GameNotStartedData::MakePacket() {
  return Packet(PacketType::GAME_NOT_STARTED);
}

int Packet::GameNotStartedData::GetID() const {
  return TypeID<BaseT>::GetID<GameNotStartedData>();
}

Packet Packet::GameStartedData::MakePacket() {
  return Packet(PacketType::GAME_STARTED);
}

int Packet::GameStartedData::GetID() const {
  return TypeID<BaseT>::GetID<GameStartedData>();
}

Packet::GameServerAddressData::GameServerAddressData(int host, int port) : host(
  host), port(port) {
}

Packet Packet::GameServerAddressData::MakePacket() {
  Packet res(PacketType::GAME_SERVER_ADDRESS);
  res.Append((char*) &host, sizeof(int));
  res.Append((char*) &port, sizeof(int));
  return res;
}

int Packet::GameServerAddressData::GetID() const {
  return TypeID<BaseT>::GetID<GameServerAddressData>();
}

Packet Packet::StartGameData::MakePacket() {
  return Packet(PacketType::START_GAME);
}

int Packet::StartGameData::GetID() const {
  return TypeID<BaseT>::GetID<StartGameData>();
}
