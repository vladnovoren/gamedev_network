#include "Host/Events.hpp"
#include "Host/BaseEventHandler.hpp"

Host::BaseEvent::~BaseEvent() noexcept = default;

void Host::NoneEvent::Handle(Host::BaseEventHandler& handler) {
  handler.Handle(*this);
}

Host::NonEmptyEvent::NonEmptyEvent(Peer sender) : sender_(sender) {}

Host::ConnectEvent::ConnectEvent(Peer sender) : NonEmptyEvent(sender) {}

void Host::ConnectEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}

Host::DisconnectEvent::DisconnectEvent(Peer sender) : NonEmptyEvent(sender) {}

void Host::DisconnectEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}

Host::ReceiveEvent::ReceiveEvent(Peer sender, int channel_id, Packet packet)
  : NonEmptyEvent(sender), channel_id_(channel_id), packet_(packet) {
}

void Host::ReceiveEvent::Handle(BaseEventHandler& handler) {
  handler.Handle(*this);
}