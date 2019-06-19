//接收端
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include<ctype.h>
#define BUFLEN 60
//将十六进制的字符串转换成整数  
int htoi(char s[])  
{  
    int i = 0;  
    int n = 0;  
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))  
    {  
        i = 2;  
    }  
    else  
    {  
        i = 0;  
    }  
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)  
    {  
        if (tolower(s[i]) > '9')  
        {  
            n = 16 * n + (10 + tolower(s[i]) - 'a');  
        }  
        else  
        {  
            n = 16 * n + (tolower(s[i]) - '0');  
        }  
    }  
    return n;  
}  

void  getMac(int src[]) {
    //网卡的ｍａｃ
    const char* mac = "90:94:97:10:fa:bd" ;
    int len = strlen(mac) ;
    int up = 0 ;
    char buf[10] ;
    bzero(buf, 10) ;
    int count = 0 ;
    while(1) {
        int flag = 0 ;
        if(*mac == ':') {
            src[count] = htoi(buf) ;
            count ++ ;
            up = 0 ;
            flag = 1 ;  
        }
        if(flag == 1) {
            mac++ ;
        }

        buf[up] = *mac ;
        mac++ ;
        up++ ;
        len--;
        if(len == 0) {
            break ;
        }
    }
    src[count] = htoi(buf) ;
    count ++ ;
    for(int  i= 0 ;  i< count; i++) {
        printf("%x ", src[i]) ;
    }
}

int main(int argc,char** argv){
    int i,skfd,n;
    char buf[ETH_FRAME_LEN]={0};
    struct ethhdr *eth;
    struct ether_arp *arp;
    struct sockaddr_ll fromaddr;
    struct ifreq ifr;

    unsigned char src_mac[ETH_ALEN]={0};

    if(2 != argc){
        printf("Usage: %s netdevName\n",argv[0]);
        exit(1);
    }

    //只接收发给本机的ARP报文
    if(0>(skfd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP)))){
        perror("Create Error");
        exit(1);
    }

    bzero(&fromaddr,sizeof(fromaddr));
    bzero(&ifr,sizeof(ifr));
    strcpy(ifr.ifr_name,argv[1]);

    //获取接口索引
    if(-1 == ioctl(skfd,SIOCGIFINDEX,&ifr)){
        perror("get dev index error:");
        exit(1);
    }

    fromaddr.sll_ifindex = ifr.ifr_ifindex;
    printf("interface Index:%d\n",ifr.ifr_ifindex);

    //获取接口的MAC地址
    if(-1 == ioctl(skfd,SIOCGIFHWADDR,&ifr)){
        perror("get dev MAC addr error:");
        exit(1);
    }

    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN) ; 
    printf("MAC :%02X-%02X-%02X-%02X-%02X-%02X\n",src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]);

    fromaddr.sll_family = PF_PACKET;
    fromaddr.sll_protocol=htons(ETH_P_ARP);
    fromaddr.sll_hatype=ARPHRD_ETHER;
    fromaddr.sll_pkttype=PACKET_HOST;
    fromaddr.sll_halen=ETH_ALEN;
    memcpy(fromaddr.sll_addr,src_mac,ETH_ALEN);
    bind(skfd,(struct sockaddr*)&fromaddr,sizeof(struct sockaddr));
    while(1){
        memset(buf,0,ETH_FRAME_LEN);
        n=recvfrom(skfd,buf,ETH_FRAME_LEN,0,NULL,NULL);
        
        eth=(struct ethhdr*)buf;
        arp=(struct ether_arp*)(buf+14);

        if(ntohs(arp->arp_op)==2){
            printf("Get an ARP replay!\n");
                    printf("Dest MAC:");
        for(i=0;i<ETH_ALEN;i++){
            printf("%02X-",eth->h_dest[i]);
        }
        printf("Sender MAC:");
        for(i=0;i<ETH_ALEN;i++){
            printf("%02X-",eth->h_source[i]);
        }

        printf("\n");
        printf("Frame type:%0X\n",ntohs(eth->h_proto));
        }
    }
    close(skfd);
    return 0;
}
