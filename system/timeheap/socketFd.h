#pragma once
#include<memory>
#include<string>
#include<iostream>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/uio.h>
#include<unistd.h>
#include<string.h>
#define BACKLOG 20
using namespace std ;

class SocketFd
{
public :
  //  SocketFd& operator=(SocketFd& sock) ;
public:
    SocketFd(int port , string ip) ;
    ~SocketFd() ;
public:
    int createSocket() ;
    int beginListen() ;
    int acceptClient() ;
public :
    int sfd ;
private:
    int port ;
    string ip ;
    struct sockaddr_in addr ;
};

