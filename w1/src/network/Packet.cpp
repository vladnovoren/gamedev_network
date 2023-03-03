#include "Packet.hpp"

Packet::Packet(PacketType type) {
  mem_sample_.type = type;
}

Packet::Packet(const char* in_mem, const size_t in_size) {
  assert(in_mem != nullptr);
  assert(in_size <= MAX_MEM_SIZE);

  memcpy(&mem_sample_, in_mem, in_size);
}

void* Packet::Mem() {
  return &mem_sample_;
}

const void* Packet::Mem() const {
  return &mem_sample_;
}

size_t Packet::MemSize() const {
  return sizeof(PacketType) + data_size_;
}

int Packet::Append(const char* add_data, const size_t add_size) {
  if (data_size_ + add_size > MAX_DATA_SIZE)
    return -1;

  memcpy(mem_sample_.data + data_size_, add_data, add_size);
  data_size_ += add_size;

  return 0;
}

std::unique_ptr<Packet::BaseData> Packet::ExtractData() {
  switch (mem_sample_.type) {
    case PacketType::REGISTRY:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsRegistry());
    case PacketType::KEEP_ALIVE:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsKeepAlive());
    case PacketType::MSG:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsMsg());
    case PacketType::UNDEFINED:
      std::cout << "packet type undefined\n";
      return nullptr;
  }
  return nullptr;
}

Packet::BaseData::Visitor::~Visitor() = default;

void Packet::BaseData::Visitor::Visit(RegistryData& packet_data) {
}

void Packet::BaseData::Visitor::Visit(KeepAliveData& packet_data) {
}

void Packet::BaseData::Visitor::Visit(MsgData& packet_data) {
}

Packet::RegistryData* Packet::ParseAsRegistry() {
  return new RegistryData(std::string(mem_sample_.data));
}

Packet::KeepAliveData* Packet::ParseAsKeepAlive() {
  return new KeepAliveData();
}

Packet::MsgData* Packet::ParseAsMsg() {
  return new MsgData(std::string(mem_sample_.data));
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

Packet Packet::KeepAliveData::MakePacket() {
  return Packet(PacketType::KEEP_ALIVE);
}

void Packet::KeepAliveData::Accept(Visitor& visitor) {
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

Packet Packet::BaseData::MakePacket() {
  return {};
}

void Packet::MsgData::Accept(BaseData::Visitor& visitor) {
  visitor.Visit(*this);
}

void Packet::BaseData::Accept(Visitor& visitor) {
}
