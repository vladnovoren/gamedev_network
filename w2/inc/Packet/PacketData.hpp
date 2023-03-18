#pragma once

#include "Packet.hpp"
#include "Tools/TypeID.hpp"

class Packet::BaseData {
 public:
  virtual ~BaseData() = default;

  virtual Packet MakePacket();

  [[nodiscard]] virtual int GetID() const;

 protected:
  using BaseT = BaseData;
};

class Packet::RegistryData : public Packet::BaseData {
 public:
  explicit RegistryData(std::string username);

  ~RegistryData() override = default;

  [[nodiscard]] const std::string& GetUsername() const;

  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;

 private:
  std::string username_;
};

class Packet::MsgData : public Packet::BaseData {
 public:
  explicit MsgData(std::string msg);

  ~MsgData() override = default;

  [[nodiscard]] const std::string& GetMsg() const;

  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;

 public:
  static constexpr size_t MAX_MSG_LEN = Packet::MAX_DATA_SIZE;
 private:
  std::string msg_;
};

class Packet::GameNotStartedData : public Packet::BaseData {
 public:
  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;
};

class Packet::GameStartedData : public Packet::BaseData {
 public:
  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;
};

class Packet::GameServerAddressData : public Packet::BaseData {
 public:
  GameServerAddressData(int host, int port);

  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;

  int host;
  int port;
};

class Packet::StartGameData : public Packet::BaseData {
 public:
  Packet MakePacket() override;

  [[nodiscard]] int GetID() const override;
};