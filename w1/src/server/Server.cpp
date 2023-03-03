#include "Server.hpp"

Server::Server(const std::string& address, const std::string& port_to_server,
               const std::string& port_to_client) :
  receiver_socket_(port_to_server), sender_socket_(address, port_to_client),
  incoming_packet_processor_(*this) {
}

void Server::Run() {
  while (true) {
    auto received_packet = receiver_socket_.Receive();
    if (received_packet.has_value()) {
      auto packet_data_p = received_packet.value().ExtractData();
      packet_data_p->Accept(incoming_packet_processor_);
    }
  }
}

Server::IncomingPacketProcessor::IncomingPacketProcessor(Server& server) :
  server_(server) {
}

void
Server::IncomingPacketProcessor::Visit(Packet::RegistryData& registry_data) {
  std::cout << "new user: \"" + registry_data.GetUsername() + "\"\n";
  server_.sender_socket_.Send(Packet::MsgData(
    "Welcome to our hashing server, " + registry_data.GetUsername() +
    "!").MakePacket());
}

void Server::IncomingPacketProcessor::Visit(
  Packet::KeepAliveData& keep_alive_data) {
}

void Server::IncomingPacketProcessor::Visit(Packet::MsgData& msg_data) {
  std::hash<std::string> hash_functor;
  auto hash = hash_functor(msg_data.GetMsg());
  server_.sender_socket_.Send(Packet::MsgData(
    "Your message hash: " + std::to_string(hash)).MakePacket());
}
