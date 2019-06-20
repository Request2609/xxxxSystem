#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<string>
#include <unistd.h>
#include<set>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include<map>
#include <net/ethernet.h>
#include"sockFd.h"
using namespace std ;

/* 以太网帧首部长度 */
#define ETHER_HEADER_LEN sizeof(struct ether_header)
/* 整个arp结构长度 */
#define ETHER_ARP_LEN sizeof(struct ether_arp)
/* 以太网 + 整个arp结构长度 */
#define ETHER_ARP_PACKET_LEN ETHER_HEADER_LEN + ETHER_ARP_LEN
/* IP地址长度 */
#define IP_ADDR_LEN 4

//接收消息
class RecvInfo {
public :
    RecvInfo() ;
    ~RecvInfo() ;
public :
    void recvMsg(int sockfd) ;
    int buildUnixSock() ;
   // int addEpoll(int fd) ;
private :
    struct ether_arp *arp_packet;
    char buf[ETHER_ARP_PACKET_LEN];
    int ret_len, i;
    map<string, string>ipMac ;
    SockFd sfd ;
} ;
