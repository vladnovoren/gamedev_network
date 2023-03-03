#include "Client.hpp"

int main(int argc, const char** argv) {
  Client client("localhost", "2023", "2024");
  client.Run();
//  const char *port = "2022";
//
//  addrinfo resAddrInfo;
//  int sfd = create_dgram_socket("localhost", port, &resAddrInfo);
//
//  if (sfd == -1)
//  {
//    printf("Cannot create a socket\n");
//    return 1;
//  }
//
//  while (true)
//  {
//    std::string input;
//    printf(">");
//    std::getline(std::cin, input);
//    ssize_t res = sendto(sfd, input.c_str(), input.size_(), 0, resAddrInfo.ai_addr, resAddrInfo.ai_addrlen);
//    if (res == -1)
//      std::cout << strerror(errno) << std::endl;
//  }
  return 0;
}
