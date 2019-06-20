#pragma once
#include <sys/types.h>          
#include <sys/socket.h>
#include<unistd.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include<stdio.h>
#include"epoll.h"

class SockFd
{
public :
    SockFd() {}
    ~SockFd() {}
public :
    int* createUnixSock() ; 
    int createPreSock() ;
    int addToEpoll() ;
    void closeUnixSock() ;
private :
    int sockFd ;
    int uSock[2] ;
};


