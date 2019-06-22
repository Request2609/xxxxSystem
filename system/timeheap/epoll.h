#pragma once
#include<sys/epoll.h>
#include<unistd.h>
#define SIZE 2048
#define EPOLLEVENTSNUM 20
class Epoll
{
public:
    Epoll() {}
    ~Epoll() {close(epFd) ;}
public :
    int epollCreate(int sockFd) ;
    int epollAdd(int epollFd, int sockFd) ;
    int epollDelete(int epollFd,int sockFd) ;
   // int getEventsFd() ;
private:
    int epFd ;
};


