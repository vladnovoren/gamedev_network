#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cassert>

static int
get_dgram_socket(addrinfo* addr, bool should_bind, addrinfo* res_addr);

int
create_dgram_socket(const char* address, const char* port, addrinfo* res_addr);

int create_dgram_sender_socket(const char* address, const char* port,
                               addrinfo* res_addr);

int create_dgram_receiver_socket(const char* port);