#include "network_utilities.h"

bool ping_test(const char* ip_address)
{
    // Declare and initialize variables

    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    // Validate the parameters
    ipaddr = inet_addr(ip_address);

    if (ipaddr == INADDR_NONE) {
        printf("Invalid IP address - %s\n", ip_address);
    }

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        printf("\tUnable to open handle.\n");
        printf("IcmpCreatefile returned error: %ld\n", GetLastError());
        free(ReplyBuffer);
        return false;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*)malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        printf("\tUnable to allocate memory\n");
        return false;
    }

    // PING
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
    if (dwRetVal != 0) {
        free(ReplyBuffer);
        return true;
    }
    else {
        free(ReplyBuffer);
        return false;
    }

    free(ReplyBuffer);
    return false;
}

int cidr_to_ip_and_mask(const char *cidr, uint32_t* ip, uint32_t* mask)
{
    uint8_t a, b, c, d, bits;
    if (sscanf(cidr, "%hhu.%hhu.%hhu.%hhu/%hhu", &a, &b, &c, &d, &bits) < 5) {
        return -1; /* didn't convert enough of CIDR */
    }
    if (bits > 32) {
        return -1; /* Invalid bit count */
    }
    *ip = (a << 24UL) | (b << 16UL) | (c << 8UL) | (d);
    *mask = (0xFFFFFFFFUL << (32 - bits)) & 0xFFFFFFFFUL;
    return 0;
}

char* int_to_string_ip(unsigned int ip)
{
    char* ret = new char[16];
    sprintf(ret, "%d.%d.%d.%d", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, (ip) & 0xFF);
    return ret;
}