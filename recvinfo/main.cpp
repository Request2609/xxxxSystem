#include"sendInfo.h"
#include"epoll.h"
#include"sockFd.h"
#include"recvInfo.h"
#include<map>
#include<thread>
#include<signal.h>
using namespace std ;

 /*//检测获取发生事件的描述符
int  getEventsFd() {
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
}
*/



int main(int argc, const char *argv[])
{

    if (argc != 2)
    {
        printf("usage:%s device_name dst_ip\n", argv[0]);
        exit(1);
    }
    
    int* uFd ;
    SockFd sfd ;
    //创建原始套接字
    sfd.createPreSock() ;
    
    map<string, string>ipMac ;
    SockFd sock ; 
    uFd = sock.createUnixSock() ;
    int preSock = sock.createPreSock() ;
    if(preSock < 0) {
        exit(1) ;
    }
    
    
    thread recvProcess(recvMsg, ref(ipMac), sock) ;
    char ipList[256][128] ;
    //获取相同局域网下的ip列表
    getIpList(ipList) ;
    int ret = sendRequest(sock, ipList, argv[1]) ;
    if(ret < 0) {
        return 0 ;
    }

    recvProcess.join() ;
    return 0;
}



