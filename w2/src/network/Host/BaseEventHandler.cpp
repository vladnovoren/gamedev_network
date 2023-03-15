#include "Host/BaseEventHandler.hpp"

void Host::BaseEventHandler::Handle(const NoneEvent&) {}

void Host::BaseEventHandler::Handle(const ConnectEvent&) {}

void Host::BaseEventHandler::Handle(const DisconnectEvent&) {}

void Host::BaseEventHandler::Handle(const ReceiveEvent&) {}
