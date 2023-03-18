#include "Packet/Packet.hpp"
#include "Packet/PacketData.hpp"

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

std::unique_ptr<Packet::BaseData> Packet::ExtractData() const {
  switch (mem_sample_.type) {
    case PacketType::REGISTRY:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsRegistry());
    case PacketType::MSG:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsMsg());
    case PacketType::GAME_NOT_STARTED:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsGameNotStarted());
    case PacketType::GAME_STARTED:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsGameStarted());
    case PacketType::GAME_SERVER_ADDRESS:
      return std::unique_ptr<BaseData>(
        (BaseData*) ParseAsGameServerAddressData());
    case PacketType::START_GAME:
      return std::unique_ptr<BaseData>((BaseData*) ParseAsStartGameData());
    case PacketType::UNDEFINED:
      std::cout << "packet type undefined\n";
      return nullptr;
  }
  return nullptr;
}

Packet::RegistryData* Packet::ParseAsRegistry() const {
  return new RegistryData(std::string(mem_sample_.data));
}

Packet::MsgData* Packet::ParseAsMsg() const {
  return new MsgData(std::string(mem_sample_.data));
}

Packet::GameNotStartedData* Packet::ParseAsGameNotStarted() const {
  return new GameNotStartedData();
}

Packet::GameStartedData* Packet::ParseAsGameStarted() const {
  return new GameStartedData();
}

Packet::GameServerAddressData* Packet::ParseAsGameServerAddressData() const {
  auto address_ptr = (Address*) mem_sample_.data;
  return new GameServerAddressData(address_ptr->host, address_ptr->port);
}

Packet::StartGameData* Packet::ParseAsStartGameData() const {
  return new StartGameData();
}
