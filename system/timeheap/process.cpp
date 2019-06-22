#include"process.h"
#include"printError.h"

Process::Process(string ips, int ports):ip(ips), port(ports) {
    //创建好套接字对象
    sock = make_shared<SocketFd>(ports, ips) ;
    //将服务器端套接字注册到ｅｐｏｌｌ事件集合中
    epFd = epoll.epollCreate(sock->sfd) ;
}

//开始监听事件
int Process :: start() {
    struct epoll_event allRes[EPOLLEVENTSNUM] ;
    while(1) {
        int ret = epoll_wait(epFd, allRes, EPOLLEVENTSNUM, -1) ;
        if(ret < 0) {
            printError(__LINE__, __FILE__) ;
        }
        for(int i=0; i<ret; i++) {
            int fd = allRes[i].data.fd ;
            if(fd == sock->sfd) {
                int connFd = sock->acceptClient() ;
                epoll.epollAdd(epFd, connFd) ;
            }
            //可读事件
            if(allRes[i].events&EPOLLIN) {
                      
            }
            //可写事件
            if(allRes[i].events&EPOLLOUT) {
                
            }
            
        }
    }
}

