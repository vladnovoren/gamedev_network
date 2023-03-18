#include "Host/Events.hpp"
#include "Host/BaseEventHandler.hpp"

Host::BaseEvent::~BaseEvent() noexcept = default;

int Host::BaseEvent::GetID() const {
  return TypeID<BaseT>::GetID<BaseEvent>();
}

void Host::NoneEvent::Handle(Host::BaseEventHandler& handler) {
  handler.Handle(*this);
}

int Host::NoneEvent::GetID() const {
  return TypeID<BaseT>::GetID<NoneEvent>();
}

Host::NonEmptyEvent::NonEmptyEvent(Peer sender) : sender_(sender) {}

int Host::NonEmptyEvent::GetID() const {
  return TypeID<BaseT>::GetID<NonEmptyEvent>();
}

Host::ConnectEvent::ConnectEvent(Peer sender) : NonEmptyEvent(sender) {}

void Host::ConnectEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}

int Host::ConnectEvent::GetID() const {
  return TypeID<BaseT>::GetID<ConnectEvent>();
}

Host::DisconnectEvent::DisconnectEvent(Peer sender) : NonEmptyEvent(sender) {}

void Host::DisconnectEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}

int Host::DisconnectEvent::GetID() const {
  return TypeID<BaseT>::GetID<DisconnectEvent>();
}

Host::ReceiveEvent::ReceiveEvent(Peer sender, int channel_id, Packet packet)
  : NonEmptyEvent(sender), channel_id(channel_id), packet(packet) {
}

void Host::ReceiveEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}

int Host::ReceiveEvent::GetID() const {
  return TypeID<BaseT>::GetID<ReceiveEvent>();
}