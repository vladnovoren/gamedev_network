#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cassert>

bool dgram_recv_socket_addr_filter(addrinfo* addr_info);

int create_dgram_sender_socket(const char* address, const char* port, addrinfo* recv_addr);

int get_dgram_sender_socket(addrinfo* recv_addr_list, addrinfo* recv_addr);

int create_dgram_receiver_socket(const char* port);

int get_dgram_receiver_socket(addrinfo* recv_addr_list);