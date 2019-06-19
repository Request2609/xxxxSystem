#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <net/ethernet.h>
#define BUFLEN 42
int main()
{
    int skfd, n ;
    char buf[BUFLEN] = {0} ;
    struct ether_header eh ;
    struct ether_arp *arp ;
    struct sockaddr_ll toaddr ;
    struct in_addr target, srcIp ;
    return 0;
}

