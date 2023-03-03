#include "Server.hpp"

Server::Server(const std::string& address, const std::string& port_to_server,
               const std::string& port_to_client) :
  receiver_socket_(port_to_server), sender_socket_(address, port_to_client),
  incoming_package_processor_(*this) {
}

void Server::Run() {
  while (true) {
    auto received_package = receiver_socket_.Receive();
    if (received_package.has_value()) {
      auto package_data_p = received_package.value().ExtractData();
      package_data_p->Accept(incoming_package_processor_);
    }
  }
}

Server::IncomingPackageProcessor::IncomingPackageProcessor(Server& server) :
  server_(server) {
}

void
Server::IncomingPackageProcessor::Visit(Package::RegistryData& registry_data) {
  std::cout << "new user: \"" + registry_data.GetUsername() + "\"\n";
  server_.sender_socket_.Send(Package::MsgData(
    "Welcome to our hashing server, " + registry_data.GetUsername() +
    "!").MakePackage());
}

void Server::IncomingPackageProcessor::Visit(
  Package::KeepAliveData& keep_alive_data) {
}

void Server::IncomingPackageProcessor::Visit(Package::MsgData& msg_data) {
  std::hash<std::string> hash_functor;
  auto hash = hash_functor(msg_data.GetMsg());
  server_.sender_socket_.Send(Package::MsgData(
    "Your message hash: " + std::to_string(hash)).MakePackage());
}
