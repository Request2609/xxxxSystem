#pragma once
#include"socketFd.h"
#include"epoll.h"

class Process
{
public:
    Process(string ips, int ports) ;
    ~Process() {}
public:
    int start() ;
private:
    string ip ;
    int port ;
    shared_ptr<SocketFd>sock ;
    Epoll epoll ;
    int epFd ;
};

