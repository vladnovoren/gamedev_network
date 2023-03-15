#include "Packet/PacketData.hpp"

Packet::BaseData::Visitor::~Visitor() = default;

void Packet::BaseData::Visitor::Visit(RegistryData&) {}

void Packet::BaseData::Visitor::Visit(MsgData&) {}

void Packet::BaseData::Visitor::Visit(GameNotStartedData&) {}

void Packet::BaseData::Visitor::Visit(GameStartedData&) {}

void Packet::BaseData::Visitor::Visit(GameServerAddressData&) {}


Packet Packet::BaseData::MakePacket() {
  return {};
}

void Packet::BaseData::Accept(Visitor&) {
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

void Packet::RegistryData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
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

void Packet::MsgData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
}

Packet Packet::GameNotStartedData::MakePacket() {
  return Packet(PacketType::GAME_NOT_STARTED);
}

void Packet::GameNotStartedData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
}

Packet Packet::GameStartedData::MakePacket() {
  return Packet(PacketType::GAME_STARTED);
}

void Packet::GameStartedData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
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

void Packet::GameServerAddressData::Accept(Visitor& visitor) {
  visitor.Visit(*this);
}

