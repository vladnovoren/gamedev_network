#include "Client.hpp"

Client::Client(std::string  username, const std::string& address,
               const std::string& to_server_port, const std::string& to_client_port) :
  username_(std::move(username)), sender_socket_(address, to_server_port),
  receiver_socket_(to_client_port) {
}

void Client::Run() {

}

void Client::Register() {

}
