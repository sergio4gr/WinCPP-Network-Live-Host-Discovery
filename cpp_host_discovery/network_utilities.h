#ifndef NETWORK_UTILITIES_H
#define NETWORK_UTILITIES_H

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <cstdint>


#pragma warning(disable:4996) 
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

bool ping_test(const char* ip_address);
int cidr_to_ip_and_mask(const char* cidr, uint32_t* first_ip, uint32_t* last_ip);
char* int_to_string_ip(unsigned int ip);

#endif