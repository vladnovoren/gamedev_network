#pragma once

#include <string>
#include <cstring>
#include <cassert>
#include <memory>
#include <iostream>
#include "Addresses.hpp"
#include <enet/enet.h>

enum class PacketType {
  UNDEFINED,
  REGISTRY,
  MSG,
  GAME_NOT_STARTED,
  GAME_STARTED,
  GAME_SERVER_ADDRESS
};

class Packet {
 public:
  class BaseData;

  class RegistryData;

  class MsgData;

  class GameNotStartedData;

  class GameStartedData;

  class GameServerAddressData;

 public:
  Packet() = default;

  explicit Packet(PacketType type);

  Packet(const char* in_mem, size_t size);

  [[nodiscard]] void* Mem();

  [[nodiscard]] const void* Mem() const;

  [[nodiscard]] size_t MemSize() const;

  int Append(const char* add_data, size_t add_size);

  [[nodiscard]] std::unique_ptr<BaseData> ExtractData() const;

 public:
  static constexpr size_t MAX_DATA_SIZE = 1000;

 private:
  [[nodiscard]] RegistryData* ParseAsRegistry() const;

  [[nodiscard]] MsgData* ParseAsMsg() const;

  [[nodiscard]] GameNotStartedData* ParseAsGameNotStarted() const;

  [[nodiscard]] GameStartedData* ParseAsGameStarted() const;

  [[nodiscard]] GameServerAddressData* ParseAsGameServerAddressData() const;

 private:
  struct MemSample {
    PacketType type = PacketType::UNDEFINED;
    char data[MAX_DATA_SIZE + 1] = {};
  };
 public:
  static constexpr size_t MAX_MEM_SIZE = sizeof(MemSample) - 1;

 private:
  MemSample mem_sample_ = {};
  size_t data_size_ = 0;
};
