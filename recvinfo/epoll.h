#pragma once
#include<sys/epoll.h>
#define SIZE 2048
#define N 20
class Epoll
{
public:
    Epoll() {}
    ~Epoll() {}
public :
    int epollCreate(int sockFd) ;
    int epollAdd(int epollFd, int sockFd) ;
    int epollDelete(int epollFd,int sockFd) ;
   // int getEventsFd() ;
private:
    int epFd ;
};


