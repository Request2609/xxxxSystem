#include"sockFd.h"

//创建原始套接字
int SockFd ::  createPreSock() {

    if ((sockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
        return -1 ;
    return sockFd ;
}

//关掉套接字
void SockFd :: closeUnixSock() {
    close(uSock[1]) ;
    close(uSock[0]) ;
}

//创建匿名ｕｎｉｘ demian套接字
int* SockFd :: createUnixSock() {
    int ret = socketpair(AF_UNIX, SOCK_DGRAM, 0, uSock) ;
    if(ret < 0) {
        printf("创建Unix域\n") ;
        return NULL ;
    }    
    return uSock ;
}

//将Unix 域套接字fd[1]加入到epoll中
int SockFd :: addToEpoll() {
    Epoll ep ;
    //将套接字加入到Epoll中
    int epFd = ep.epollCreate(uSock[1]) ;
    if(epFd < 0) {
        return -1 ;
    }
    return 1 ;
}
