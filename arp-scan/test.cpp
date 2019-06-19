#include <iostream>
#include<wait.h>
#include<unistd.h>
using namespace std ;

int main()
{
    system("") ;
   /* pid = fork() ;
    int status  = -1 ;
    if(pid == 0) {
         uid_t uid = getuid() ;
         if(setuid(0)) {
            execl("/usr/share/arp-scan", "arp-scan", "-l", NULL) ;
         }
         if(setuid(uid)) {
            cout << "恢复普通用户身份!" << endl ;          
         }
    }
    else {
        wait(&status) ;
    }*/
    return 0;
}

