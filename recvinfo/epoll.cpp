#include<iostream>
#include"epoll.h"
//创建epoll句柄
int Epoll :: epollCreate(int sockFd) {
    
    epFd = epoll_create(SIZE) ;
    struct epoll_event ev ;
    ev.events = EPOLLET|EPOLLIN ;
    ev.data.fd = sockFd ;
    int ret = epoll_ctl(epFd, EPOLL_CTL_ADD, sockFd, &ev) ;
    if(ret < 0) {
        printf("创建句柄出错！\n") ;
        return -1;
    }
    return epFd ;
}

//添加套接字
int Epoll :: epollAdd(int epollFd, int sockFd) {
    
    struct epoll_event ev ;
    ev.data.fd = sockFd ;
    ev.events = EPOLLIN|EPOLLET ;
    ev.data.fd = sockFd ;
    int ret = epoll_ctl(epollFd, EPOLL_CTL_ADD, sockFd, &ev) ;
    if(ret < 0) {
        printf("添加套接字失败！") ;
        return -1 ;
    }
    return 1 ;
}

//删除套接字失败:
int Epoll :: epollDelete(int epollFd, int sockFd) {

    struct epoll_event ev ;
    ev.data.fd = sockFd ;
    int ret = epoll_ctl(epollFd, EPOLL_CTL_DEL, sockFd, &ev) ; 
    if(ret < 0) {
        printf("删除消息失败！") ;
        return  -1;
    }
    return 1 ;
}
/*

//检测获取发生事件的描述符
int Epoll :: getEventsFd() {
    struct epoll_event allRes[N] ;
    while(1) {
        int ret = epoll_wait(epFd, allRes, N, -1) ;
        if(ret < 0) {
            printf("epoll wait err") ;
            exit(1) ;
        }
        for(int i=0; i<ret; i++) {
            int fd = allRes[i].data.fd ;
            //可读事件
            if(allRes[i].events&EPOLLIN) {
                   
            }
            //可写事件
            if(allRes[i].events&EPOLLOUT) {
            }
        }
}
*/
