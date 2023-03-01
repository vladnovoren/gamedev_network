#include "socket_init.hpp"

bool dgram_recv_socket_addr_filter(addrinfo* addr_info) {
  if (addr_info->ai_family != AF_INET || addr_info->ai_socktype != SOCK_DGRAM ||
      addr_info->ai_protocol != IPPROTO_UDP)
    return false;
  return true;
}

int create_dgram_sender_socket(const char* address, const char* port, addrinfo* recv_addr) {
  assert(address != nullptr);
  assert(port != nullptr);
  assert(recv_addr != nullptr);

  addrinfo hints = {};
  memset(&hints, 0, sizeof(addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  addrinfo* recv_addr_list = nullptr;
  if (getaddrinfo(address, port, &hints, &recv_addr_list) != 0)
    return 1;

  return get_dgram_sender_socket(recv_addr_list, recv_addr);
}

int get_dgram_sender_socket(addrinfo* recv_addr_list, addrinfo* recv_addr) {
  assert(recv_addr_list != nullptr);
  assert(recv_addr != nullptr);

  for (addrinfo* curr_addr = recv_addr_list; curr_addr != nullptr; curr_addr = curr_addr->ai_next) {
    if (dgram_recv_socket_addr_filter(curr_addr))
      continue;

    int sfd = socket(curr_addr->ai_family, curr_addr->ai_socktype, curr_addr->ai_protocol);
    if (sfd == -1)
      continue;

    fcntl(sfd, F_SETFL, O_NONBLOCK);

    int true_val = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    *recv_addr = *curr_addr;
    return sfd;
  }

  return -1;
}

int create_dgram_receiver_socket(const char* port) {
  assert(port != nullptr);

  addrinfo hints = {};
  memset(&hints, 0, sizeof(addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  addrinfo *recv_addr_list = nullptr;
  if (getaddrinfo(nullptr, port, &hints, &recv_addr_list) != 0)
    return 1;

  return get_dgram_receiver_socket(recv_addr_list);
}

int get_dgram_receiver_socket(addrinfo* recv_addr_list) {
  assert(recv_addr_list != nullptr);

  for (addrinfo* curr_addr = recv_addr_list; curr_addr != nullptr; curr_addr = curr_addr->ai_next)
  {
    if (dgram_recv_socket_addr_filter(curr_addr))
      continue;

    int sfd = socket(curr_addr->ai_family, curr_addr->ai_socktype, curr_addr->ai_protocol);
    if (sfd == -1)
      continue;

    fcntl(sfd, F_SETFL, O_NONBLOCK);

    int true_val = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    if (bind(sfd, curr_addr->ai_addr, curr_addr->ai_addrlen) == 0)
      return sfd;

    close(sfd);
  }

  return -1;
}