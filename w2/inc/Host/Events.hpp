#pragma once

#include "Host.hpp"
#include "Tools/TypeID.hpp"

class Host::BaseEvent {
 public:
  virtual ~BaseEvent() noexcept = 0;

  virtual void Handle(BaseEventHandler& handler) = 0;

  [[nodiscard]] virtual int GetID() const;
 protected:
  using BaseT = BaseEvent;
};

class Host::NoneEvent : public BaseEvent {
 public:
  ~NoneEvent() noexcept override = default;

  void Handle(BaseEventHandler& handler) override;

  [[nodiscard]] int GetID() const override;
};

class Host::NonEmptyEvent : public BaseEvent {
 public:
  explicit NonEmptyEvent(Peer sender);

  ~NonEmptyEvent() noexcept override = default;

  Peer sender_;

  [[nodiscard]] int GetID() const override;
};

class Host::ConnectEvent : public NonEmptyEvent {
 public:
  explicit ConnectEvent(Peer sender);

  ~ConnectEvent() noexcept override = default;

  void Handle(BaseEventHandler& handler) override;

  [[nodiscard]] int GetID() const override;
};

class Host::DisconnectEvent : public NonEmptyEvent {
 public:
  explicit DisconnectEvent(Peer sender);

  ~DisconnectEvent() noexcept override = default;

  void Handle(BaseEventHandler& handler) override;

  [[nodiscard]] int GetID() const override;
};

class Host::ReceiveEvent : public NonEmptyEvent {
 public:
  ReceiveEvent(Peer sender, int channel_id, Packet packet);

  int channel_id = 0;
  Packet packet;

  ~ReceiveEvent() noexcept override = default;

  void Handle(BaseEventHandler& handler) override;

  [[nodiscard]] int GetID() const override;
};