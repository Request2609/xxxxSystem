#include"sendInfo.h"
#include"epoll.h"
#include"sockFd.h"
#include"process.h"
#include"recvInfo.h"
#include<thread>
#include<signal.h>
using namespace std ;

//业务很简单，
//发送广播请求的业务，开线程
//接收回送请求的线程
//等接收到回送请求的线程响应后，处理业务的线程开始进行比较，并统计数据
int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        printf("usage:%s device_name dst_ip\n", argv[0]);
        exit(1);
    }
    //连接redis数据库
    
    //设置信号处理函数,及时终止线程
    signal(SIGINT, sighandle) ;
    //Unix域套接字对
    int* uFd ;
    //创建套接字对象
    SockFd sock ; 
    //创建匿名Unix域套接字，赶回指向套接字数组的头指针
    uFd = sock.createUnixSock() ;
    //创建原始套接字
    int preSock = sock.createPreSock() ;
    if(preSock < 0) {
        exit(1) ;
    }
    char ipList[256][128] ;
    //获取相同局域网下的ip列表
    SendInfo si ;
    RecvInfo ri ;
    map<string, pair<string, long>> nameMac ;
    si.getIpList(ipList) ;
    getNameMac(nameMac) ;
    //负责接收数据
    thread re(recvMsg,ri, preSock) ;
    //负责发送数据
    si.sendRequest(preSock, ipList, argv[1]) ;
    //负责比对信息
    thread execCbFunc(cbFunc, ri, nameMac) ;
    
    re.join() ;
    execCbFunc.join() ;
    return 0;
}


