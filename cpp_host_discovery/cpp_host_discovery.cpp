// cpp_host_discovery.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <iostream>
#include "network_utilities.h"

void workThreadFunction(std::atomic<int>* progress) {
    for (int i = 0; i < 100; i++)
    {
        progress->store(i);  // updates the variable safely
        chrono::milliseconds dura(2000);
        this_thread::sleep_for(dura); // Sleeps for a bit
    }
    return;
}


int main(int argc, char **argv)
{
    const char* target_cidr = "192.168.1.0/24";
    bool include_network_ip = false;
    bool include_broadcast_ip = false;
    bool dns_resolution = false;
    uint32_t n_threads = 1;

    std::cout << R"(
 _   _      _                      _      _     _           
| \ | | ___| |___      _____  _ __| | __ | |   (_)_   _____ 
|  \| |/ _ \ __\ \ /\ / / _ \| '__| |/ / | |   | \ \ / / _ \
| |\  |  __/ |_ \ V  V / (_) | |  |   <  | |___| |\ V /  __/
|_| \_|\___|\__| \_/\_/ \___/|_|  |_|\_\ |_____|_| \_/ \___|
                                                            
 _   _           _     ____  _                                   
| | | | ___  ___| |_  |  _ \(_)___  ___ _____   _____ _ __ _   _ 
| |_| |/ _ \/ __| __| | | | | / __|/ __/ _ \ \ / / _ \ '__| | | |
|  _  | (_) \__ \ |_  | |_| | \__ \ (_| (_) \ V /  __/ |  | |_| |
|_| |_|\___/|___/\__| |____/|_|___/\___\___/ \_/ \___|_|   \__, |
                                                           |___/ 
_________________________________________________________________
    )" << '\n';

    std::cout << "  :: Target Network               : " << target_cidr << std::endl;
    std::cout << "  :: Number of threads            : " << n_threads << std::endl;
    std::cout << "  :: Include Network Address      : " << include_network_ip << std::endl;
    std::cout << "  :: Include Broadcast Address    : " << include_broadcast_ip << std::endl;
    std::cout << "  :: Name Resolution              : " << dns_resolution << std::endl;
    std::cout << R"(
_________________________________________________________________
    )" << '\n';

    uint32_t ip;
    uint32_t mask;
    cidr_to_ip_and_mask(target_cidr, &ip, &mask);
    uint32_t first_ip = ip & mask;
    uint32_t final_ip = first_ip | ~mask;
    

    if (!include_network_ip) { first_ip++; };
    if (!include_broadcast_ip) { final_ip--; };

    uint32_t test_ip = first_ip;

    while (test_ip <= final_ip) {
        char* test_ip_str = int_to_string_ip(test_ip);
        bool ping_result = ping_test(test_ip_str);
        if (ping_result) {
            std::cout << "[+] " << test_ip_str << std::endl;
        }
        test_ip++;
    }
}