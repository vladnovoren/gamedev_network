#pragma once

#include "Host.hpp"

class Host::BaseEventHandler {
 public:
  virtual void Handle(const NoneEvent& none);

  virtual void Handle(const ConnectEvent& connect_event);

  virtual void Handle(const DisconnectEvent& disconnect_event);

  virtual void Handle(const ReceiveEvent& receive_event);
};