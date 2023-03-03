#include "Client.hpp"

Client::Client(const std::string& address, const std::string& to_server_port,
               const std::string& to_client_port) :
  sender_socket_(address, to_server_port),
  receiver_socket_(to_client_port), incoming_package_procesoor_(*this) {
}

void Client::Run() {
  Register();

  std::thread queries_thread(&Client::Queries, *this);
  std::thread keep_alive_thread(&Client::KeepAlive, *this);

  queries_thread.join();
  keep_alive_thread.join();
}

void Client::Register() {
  std::cout << "Enter your username: ";
  std::cin >> username_;

  sender_socket_.Send(Package::RegistryData(username_).MakePackage());
  bool confirmed = false;
  while (!confirmed) {
    auto confirm_package = receiver_socket_.Receive();
    if (confirm_package.has_value()) {
      auto package_data_p = confirm_package->ExtractData();
      package_data_p->Accept(incoming_package_procesoor_);
      confirmed = true;
    }
  }
}

void Client::KeepAlive() {
  while (true) {
    std::this_thread::sleep_for(1000ms);
    sender_socket_.Send(Package::KeepAliveData().MakePackage());
  }
}

void Client::Queries() {
  std::string str;
  while (true) {
    std::cout << ">";
    std::getline(std::cin >> std::ws, str);
    sender_socket_.Send(Package::MsgData(str).MakePackage());
    bool received = false;
    while (!received) {
      auto answer = receiver_socket_.Receive();
      if (answer.has_value()) {
        auto package_data_p = answer->ExtractData();
        package_data_p->Accept(incoming_package_procesoor_);
        received = true;
      }
    }
  }
}

Client::IncomingPackageProcessor::IncomingPackageProcessor(Client& client) :
  client_(client) {
}

void Client::IncomingPackageProcessor::Visit(Package::MsgData& msg_data) {
  std::cout << msg_data.GetMsg() << '\n';
}
