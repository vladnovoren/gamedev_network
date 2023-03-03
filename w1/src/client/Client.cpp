#include "Client.hpp"

Client::Client(const std::string& address, const std::string& to_server_port,
               const std::string& to_client_port) :
  sender_socket_(address, to_server_port),
  receiver_socket_(to_client_port), incoming_packet_procesoor_(*this) {
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

  sender_socket_.Send(Packet::RegistryData(username_).MakePacket());
  bool confirmed = false;
  while (!confirmed) {
    auto confirm_packet = receiver_socket_.Receive();
    if (confirm_packet.has_value()) {
      auto packet_data_p = confirm_packet->ExtractData();
      packet_data_p->Accept(incoming_packet_procesoor_);
      confirmed = true;
    }
  }
}

void Client::KeepAlive() {
  while (true) {
    std::this_thread::sleep_for(1000ms);
    sender_socket_.Send(Packet::KeepAliveData().MakePacket());
  }
}

void Client::Queries() {
  std::string str;
  while (true) {
    std::cout << ">";
    std::getline(std::cin >> std::ws, str);
    sender_socket_.Send(Packet::MsgData(str).MakePacket());
    bool received = false;
    while (!received) {
      auto answer = receiver_socket_.Receive();
      if (answer.has_value()) {
        auto packet_data_p = answer->ExtractData();
        packet_data_p->Accept(incoming_packet_procesoor_);
        received = true;
      }
    }
  }
}

Client::IncomingPacketProcessor::IncomingPacketProcessor(Client& client) :
  client_(client) {
}

void Client::IncomingPacketProcessor::Visit(Packet::MsgData& msg_data) {
  std::cout << msg_data.GetMsg() << '\n';
}
