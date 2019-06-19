#include <iostream>
#include<unistd.h>
#include<sys/time.h>
#include <sys/socket.h>
#include<signal.h>
#include<thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include <netdb.h>
using namespace std ;
#define LEN 1024 
#define PACKET_LEN 64 
void sendMsg() ;
void recvMsg() ;
void icmpPack(struct icmp* icmpHdr, int seq, int length) ;
int icmpUnpack(char* buf, int len) ;
struct timeval calTimeOffset(struct timeval begin, struct timeval end) ;
void icmpSighandle(int signo) ;
void pingShowStatus() ;
unsigned short calChkSum(unsigned short* addr, int len) ;

struct pingPacket {
    int seq ;
    int flag ;
    struct timeval beginTime ;
    struct endTime ;
} ;

class info {

public :
    pingPacket packet[PACKET_LEN] ;
    struct sockaddr_in dest ;
    struct timeval endTime ;
    struct timeval startTime ;
    struct timeval timeInterval ;
    pid_t pid  ;
    int sendCount ;
    int recvCount ;
    int rawSock ;
    int alive ;
} ;

info pack ; 
int main(int argc,char** argv)
{
    bzero(&pack, sizeof(pack)) ;
    int size = 128*1024 ;
    struct protoent* protocol = NULL ;
    char dest_addr_str[80] ;
    struct hostent* host = NULL ;
    
    if(argc < 2) {
        cout <<"输入格式不正确！"<<endl ;
    }
    //获取协议类型
    protocol = getprotobyname("icmp") ;
    if(protocol == NULL) {
        cout << "Fail to getprotobyname " << endl ;
        return -1 ;
    }
    memcpy(dest_addr_str, argv[1], strlen(argv[1])+1) ;
    
    pack.rawSock = socket(AF_INET, SOCK_RAW, protocol->p_proto) ;
    if(pack.rawSock < 0) {
        cout << "创建套接字失败！" << endl ;
        exit(1) ;
    } 
    pack.pid = getpid() ;
    //改变缓冲区的大小
    setsockopt(pack.rawSock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) ;
    bzero(&pack.dest, sizeof(pack.dest)) ;
    pack.dest.sin_family = AF_INET ;
    unsigned int inaddr = inet_addr(argv[1]) ;
    //输入的是域名
    if(inaddr == INADDR_NONE) {
        host = gethostbyname(argv[1]) ;
        cout << host << endl ;
        if(host == NULL) {
            cout <<  "Fail to gethostname" << endl ;
            return -1 ;
        }
        const char*ip = inet_ntoa(*(struct in_addr*)host->h_addr) ; 
        pack.dest.sin_addr.s_addr = inet_addr(ip) ;
       // memcpy((char*)&(pack.dest.sin_addr), host->h_addr, host->h_length) ;
    }
    //输入的是ip
    else {
        memcpy((char*)&pack.dest.sin_addr, &inaddr, sizeof(inaddr)) ;
    }

    signal(SIGINT, icmpSighandle) ;
    inaddr = pack.dest.sin_addr.s_addr ;
    printf("ping %s, (%d.%d.%d.%d)",dest_addr_str, inaddr&0x000000ff 
        , ((inaddr&0x0000ff00)>>8) ,(inaddr&0x00ff0000)>>16, (inaddr&0xff000000)>>24);
    pack.alive = 1  ;
    
    thread sendFor(sendMsg);
    thread recvFrom(recvMsg) ;
    sendFor.join() ;
    recvFrom.join() ;    
    pingShowStatus() ;
    close(pack.rawSock) ;
}

//接收到中断信号后，进行处理
void icmpSighandle(int signo) {   
    pack.alive = 0 ;
    gettimeofday(&pack.endTime, NULL) ;
    pack.timeInterval = calTimeOffset(pack.startTime, pack.endTime) ;
}

//显示ping的状态
void pingShowStatus() {
    long time = pack.timeInterval.tv_sec*1000 +pack.timeInterval.tv_usec/1000 ;
    printf("%d packets transemitted, %d received, %d%c packet loss, time %ld ms\n", pack.sendCount, pack.recvCount, (pack.sendCount-pack.recvCount)*100/pack.sendCount, '%', time) ; 
}

//发送消息
void sendMsg(){
    char sendBuf[128] ;
    memset(sendBuf, 0, sizeof(sendBuf)) ;
    gettimeofday(&pack.startTime, NULL) ;
    while(pack.alive) {
        int size = 0 ;
        gettimeofday(&(pack.packet[pack.sendCount].beginTime), NULL) ;
        pack.packet[pack.sendCount].flag = 1 ;
        icmpPack((struct icmp*)sendBuf, pack.sendCount, 64) ;
        size = sendto(pack.rawSock, sendBuf, 64, 0, (struct sockaddr*)&pack.dest, sizeof(pack.dest)) ;
        //记录pig包的数量
        pack.sendCount ++ ;
        if(size < 0) {
            printf("发送icmp包失败！\n") ;
            continue ;
        }
        cout << "已经发送"<<endl ;
        sleep(1) ;
    }
}

void recvMsg() {
    struct timeval tv ;
    tv.tv_usec = 200 ;//设置超时时间为200微秒
    tv.tv_sec = 0 ;
    fd_set readFd ;
    char recvBuf[512] ;
    memset(recvBuf, 0, sizeof(recvBuf)) ;
    while(pack.alive) {
        int ret = 0 ;
        FD_ZERO(&readFd) ;
        FD_SET(pack.rawSock, &readFd) ;
        ret = select(pack.rawSock+1, &readFd, NULL, NULL, &tv) ;
        switch(ret) {
            case -1 :
                printf("fail to select !\n");
                break ;
            case 0 :
                break ;
            default :
                {
                    int size = recv(pack.rawSock, recvBuf, sizeof(recvBuf), 0) ;
                    cout << "接收到的数据：" << size <<" 字节 "<< endl ;
                    if(size < 0) {
                        printf("接收数据失败！") ;
                        continue ;
                    }
                    //对于接收到的包进行解包
                    ret = icmpUnpack(recvBuf, size) ;
                    if(ret < 0) {
                        printf("包数据有错误！") ;
                        continue ;
                    }
                    pack.recvCount++ ;
                }
                break ;
        }
    }
}

int icmpUnpack(char* buf, int len) {
    int iphdrLen ;
    struct timeval beginTime, recvTime, offsetTime ;
    int rtt ;
    struct ip* ipHdr = (struct ip*) buf ;
    iphdrLen = ipHdr->ip_hl*4 ;
    struct icmp* icmp =(struct icmp*)(buf+iphdrLen) ;
    //数据包的长度小于８
    len -= iphdrLen ;
    if(len < 8) {
        printf("无效的数据包！") ;
        return -1 ;
    }
    
    //判断包是否是发出的
    if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == (pack.pid&0xffff))) {
        if(icmp->icmp_seq < 0 || icmp->icmp_seq > PACKET_LEN) {
            printf("icmp out of range!\n") ;
            return -1 ;
        }
        pack.packet[icmp->icmp_seq].flag = 0 ;
        beginTime = pack.packet[icmp->icmp_seq].beginTime ;
        gettimeofday(&recvTime, NULL) ;
        offsetTime = calTimeOffset(beginTime, recvTime) ;
        rtt = offsetTime.tv_sec*1000 + offsetTime.tv_usec/1000 ;
        printf("icmp发出去了！") ;
        printf("%d byte from %s :icmp_seq=%u, ttl= %d rtt = %d ms\n", len, inet_ntoa(ipHdr->ip_src), icmp->icmp_seq, ipHdr->ip_ttl, rtt) ; 
    }

    else {
        printf("无效的icmp包！");
        return -1 ;
    }
    return 0 ;
}   

struct timeval calTimeOffset(struct timeval begin, struct timeval end) {
    struct timeval ans ;
    ans.tv_sec = end.tv_sec - begin.tv_sec ;
    ans.tv_sec = end.tv_usec = begin.tv_usec ;
    if(ans.tv_usec < 0) {
        ans.tv_sec -- ;
        ans.tv_usec += 1000000 ;
    }
    return ans ;
}


//封装icmp包
void icmpPack(struct icmp* icmpHdr, int seq, int length) {

    icmpHdr->icmp_type = ICMP_ECHO ;
    icmpHdr->icmp_cksum = 0 ;
    icmpHdr->icmp_code = 0 ;
    icmpHdr->icmp_seq = seq ;
    icmpHdr->icmp_id = pack.pid&0xffff ;
    for(int i = 0; i< length; i++) {
        icmpHdr->icmp_data[i] = i ;
    }
    
    icmpHdr->icmp_cksum = calChkSum((unsigned short*)icmpHdr, length) ;
}

//计算校验和
unsigned short calChkSum(unsigned short* addr, int len) {
   int nleft = len ;
    int sum = 0 ;
    unsigned short* w = addr ;
    unsigned short answer = 0 ;
    //计算所有16位值的和，若长度为奇数，
    //将最后一个字节也加入到总和中
    while(nleft > 1) {
        sum = *w++ ;
        nleft -= 2 ;
    }
    if(nleft == 1) {
        *(unsigned char*)(&answer) = *(unsigned char*)w  ;
        sum += answer ;
    }
    sum = (sum>>16)+(sum&0xffff) ;
    sum += (sum >> 16) ;
    answer = ~sum ;
    return answer ;
}

