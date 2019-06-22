#include"socketFd.h"
#include"printError.h"

SocketFd :: SocketFd(int port, string ip) {
    
    sfd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sfd < 0) {
        printError(__LINE__, __FILE__) ;
    }
    bzero(&addr, sizeof(addr)) ;
    addr.sin_family = AF_INET ;
    addr.sin_addr.s_addr = inet_addr(ip.c_str()) ;
    addr.sin_port = port ;
    memset(addr.sin_zero, '\0', 8) ;
}

SocketFd :: ~SocketFd() {
    close(sfd) ;
}
/*
SocketFd :: operator=(SocketFd sf) {
    memcpy(this, sizeof(sf), &sf) ;
} */

//创建socket
int SocketFd :: createSocket() {
    sfd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sfd < 0) {
        printError(__LINE__, __FILE__) ;
    }
    return sfd ;
}


int SocketFd :: beginListen() {
    int flag = 1 ;
    int ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) ;
    if(ret < 0) {
        printError(__LINE__, __FILE__) ;
    }
    ret = bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        printError(__LINE__, __FILE__) ;
    }
    ret = listen(sfd, BACKLOG) ;
    if(ret < 0) {
        printError(__LINE__, __FILE__) ;
    }
    return 1 ;
}

//接收客户端连接
int SocketFd :: acceptClient() {
    struct sockaddr_in cli ;
    socklen_t slen = sizeof(cli) ;
    int connFd = accept(sfd, (struct sockaddr*)&addr, &slen) ;
    if(connFd < 0) {
        printError(__LINE__, __FILE__) ;
    }
    return connFd ;
}
