#pragma once
#include <sys/types.h>          
#include<iostream>
#include <sys/socket.h>
#include<unistd.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include<functional>
#include<stdio.h>
#include<memory>
#include<exception>

#define BUFFERSIZE 64

class HeapTimer ;

//用户数据
class ClientData {
public :
    ClientData() ;
    ~ClientData() ;
private :
    sockaddr_in address ;
    int sockFd ;
    char buf[BUFFERSIZE] ;
    std::shared_ptr<HeapTimer>ht ;
};

//最小堆节点元素
class HeapTimer
{
public :
    std::function<void(std::shared_ptr<ClientData>)> cbFunc ;
    //定时器生效的绝对时间
    time_t expire ;
    //用户信息
    std::shared_ptr<ClientData> userData ;
public :
    HeapTimer() ;
    HeapTimer(int delay) ;
    ~HeapTimer() ;
};

class TimeHeap {
private :
    HeapTimer ** array ;
    //对数组容量
    //堆数组的大小
    int capacity ;
    int size ;
public :
    TimeHeap() ;
    //构造函数１，初始化大小为ｃａｐ的空数组
    TimeHeap(int cap)  ;  
    TimeHeap(HeapTimer** int_array, int size, int cap) ;
    ~TimeHeap() ;
public :
    void percolateDown(int hole) ;
    void addTimer(HeapTimer* timer) ;
    void delTimer(HeapTimer* timer) ;
    void popTimer() ;
    void tick() ;
    void reSize() ;
};


