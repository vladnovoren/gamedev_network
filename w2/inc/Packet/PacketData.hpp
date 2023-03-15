#pragma once

#include "Packet.hpp"

class Packet::BaseData {
 public:
  class Visitor {
   public:
    virtual ~Visitor() = 0;

    virtual void Visit(RegistryData& packet_data);

    virtual void Visit(MsgData& packet_data);

    virtual void Visit(GameNotStartedData& packet_data);

    virtual void Visit(GameStartedData& packet_data);

    virtual void Visit(GameServerAddressData& packet_data);
  };

 public:
  virtual ~BaseData() = default;

  virtual Packet MakePacket();

  virtual void Accept(Visitor& visitor);
};

class Packet::RegistryData : public Packet::BaseData {
 public:
  explicit RegistryData(std::string username);

  ~RegistryData() override = default;

  [[nodiscard]] const std::string& GetUsername() const;

  Packet MakePacket() override;

  void Accept(Visitor& visitor) override;

 private:
  std::string username_;
};

class Packet::MsgData : public Packet::BaseData {
 public:
   explicit MsgData(std::string msg);

   ~MsgData() override = default;

   [[nodiscard]] const std::string& GetMsg() const;

   Packet MakePacket() override;

   void Accept(Visitor& visitor) override;

  public:
   static constexpr size_t MAX_MSG_LEN = Packet::MAX_DATA_SIZE;
  private:
   std::string msg_;
 };

class Packet::GameNotStartedData : public Packet::BaseData {
 public:
  Packet MakePacket() override;

  void Accept(Visitor& visitor) override;
};

class Packet::GameStartedData : public Packet::BaseData {
 public:
  Packet MakePacket() override;

  void Accept(Visitor& visitor) override;
};

class Packet::GameServerAddressData : public Packet::BaseData {
 public:
  GameServerAddressData(int host, int port);

  Packet MakePacket() override;

  void Accept(Visitor& visitor) override;

  int host;
  int port;
};