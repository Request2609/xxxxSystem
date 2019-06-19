#include<linux/icmp.h>
#include<unistd.h>
#include<sys/time.h>
#include <sys/socket.h>
#include<string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include<netinet/ip_icmp.h>
#include<stdlib.h>
#include<stdio.h>
#define PACKET_SEND_MAX_NUM 64 

typedef struct ping_packet_status {
    
    struct timeval begin_time ;
    struct timeval end_time ;
    int flag ;
    int seq ;
}ping_packet_status ;

ping_packet_status ping_packet[PACKET_SEND_MAX_NUM] ;

//计算校验和
unsigned short cal_chksum(unsigned short * addr, int len) {
    int nleft = len ;
    int sum = 0 ;
    unsigned short * w = addr ;
    unsigned short answer = 0 ;

    while(nleft > 1) {
        sum += *w++ ;
        nleft -= 2 ;
    }
    sum = (sum>>16)+(sum&0xffff) ;
    answer = ~sum ;
    return answer ;
}
//ｐｉｎｇ包的格式
void icmp_pack(struct icmp* icmphdr, int seq, int length) {

    int pid = getpid() ;
    //类型为回送请求
    icmphdr->icmp_type = ICMP_ECHO ;
    icmphdr->icmp_code = 0 ;
    icmphdr->icmp_cksum = 0 ;
    icmphdr->icmp_seq = seq ;
    icmphdr->icmp_id = pid&0xffff ;
    for(int i=0 ; i< length ; i++) {
        icmphdr->icmp_data[i] = i ;//填充数据段使得icmp报文大于64B
    }
    //计算校验和,前提是icmp_cksum提前置为０
    icmphdr->icmp_cksum = cal_chksum((unsigned short*)icmphdr, length) ;
}

//icmp 解包
int icmp_unpack(char*buf, int len) {
    
    int iphdr_len ;
    struct timeval begin_time, recv_time, offset_time ;
    int rtt ;
    struct ip* ip_hdr = (struct ip*)buf ;
    iphdr_len = ip_hdr->ip_hl*4 ;
    pid_t pid = getpid() ;
    struct icmp* icmp = (struct icmp*)(buf+iphdr_len) ;
    len -= iphdr_len ;
    if(len < 8) {//判断长度是否为ICMP包的长度
        fprintf(stderr, "invalild icmp packet. Its length is less than 8") ;
        return 0 ;
    }
    //判断该包是icmp回送回答包还是我们发出去的
    if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == (pid& 0xffff))) {
        if(icmp->icmp_seq < 0 || icmp ->icmp_seq > PACKET_SEND_MAX_NUM) {
            fprintf(stderr, "icmp packet seq is out of range!\n") ;
            return -1 ;
        }
        ping_packet[icmp->icmp_seq].flag = 0;
        begin_time = ping_packet[icmp->icmp_seq].begin_time ;
        gettimeofday(&recv_time, NULL) ;
        offset_time = cal_time_offset(begin_time, recv_time) ;
        rtt = offset_time.tv_sec*1000+offset_time.tv_usec/1000 ;
        std::cout << "数据长度：" << len << "序列号：" << inet_ntoa(ip_hdr->ip_src) << "开始时间：" << tt1 << "往返时间：" << rtt << std::endl ;
    }
    else {
        std::cout << "invalild icmp packet! Its id is not matched !"<< std::endl ;
        return -1 ;
    }
    return 0 ;
}

//发送ping包
void ping_send() {
    char send_buf[128] ;
    memset(send_buf, 0, sizeof(send_buf)) ;
    struct timeval start_time ;
    gettimeofday(&start_time, NULL) ;
    int alive = 1 ;
    int send_count = 0 ;
    while(alive) {
        int size =0 ;
        gettimeofday(&(ping_packet[send_count].begin_time), NULL) ;
        ping_packet[send_count].flag = 1 ;
        icmp_pack((struct icmp*)send_buf, send_count, 64) ;
        size = send_to(rawsock, send_buf, 64, 0, (struct sockaddr*)&dest, sizeof(dest)) ;
        send_count++ ;
        if(size < 0) {
            std::cout << "error~!" <<std::endl ;
            continue ;
        }
        sleep(1) ;
    }
}

//收报线程的创建
void ping_recv() {
    struct timeval tv ;
    tv.tv_usec = 200 ;
    tv.tv_sec = 0 ;
    fd_set read_fd ;
    char recv_buf[512] ;
    memset(recv_buf, 0, sizeof(recv_buf)) ;
    while(alive) {
        int ret = 0 ;
        FD_ZERO(&read_fd) ;
        FD_SET(rawsock, &read_fd) ;
        ret = select(rawsock+1, &read_fd, NULL, NULL, &tv) ;
        switch(ret) {
            case -1 :
                printf("fail to select!");
                break ;
            case 0 :
                break ;
            default:
            {
                int size = recv(rawsock, recv_buf, sizeof(recv_buf), 0) ;
                if(size < 0) {
                    printf("recv data fail!") ;
                    continue ;
                }
                recv_count++ ;
                }
            break ;
        }
    }
}

void icmp_sigint(int signo) {
    alive = 0 ;
    gettimeofday(&end_time, NULL) ;
    time_interval = cal_time_offset(start_time, end_time) ;
}

sigint(SIGINT, icmp_sigint) ;
