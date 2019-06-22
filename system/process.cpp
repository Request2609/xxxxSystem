#include"process.h"
condition_variable cond ;
mutex mute ;
bool notify = false ;
bool done = false ;

//回调通过信号通知后,开始比对ｍａｃ地址统计数据
//并删除比对过的map中的mac地址
void cbFunc(RecvInfo ri, map<string, pair<string, long>>nameMac) {
    while(!done) {
        //上锁保护共享map,在构造函数里加锁，析构函数解锁
        unique_lock<mutex> lk(mute) ;
        while(!notify) {
            cond.wait(lk) ;
        }
        //判断map不为空的话，就比对信息
        //并将已经比对过的信息删除掉
        while(!ri.macIp.empty()) {
            //删除比对信息
            for(pair<string, string>s : ri.macIp) {
                    if(nameMac.find(s.second) == nameMac.end()) {
                        continue ;
                    }
                    else {
                        //要是在线的话就给在线时长加五分钟
                        nameMac[s.first].second+=5 ;   
                    }
            }
            if(nameMac.size() != 0) {
                for(pair<string, pair<string, long>> s : nameMac) {
                    cout << "mac地址：" << s.first << "  姓名：" << s.second.first << "在线时长：" << s.second.second << endl ;
                }   
            }
            notify = false ;
            //通知线程接收数据
            cond.notify_one() ;
        }
    }
}

//获取姓名和ｍａｃ地址
void getNameMac(map<string, pair<string, long>>&nameMac) {
    
    ifstream inFile("info", ios::in) ;
    if(!inFile) {
        printf("请求检查该文件是否存在！") ;
        return ;
    }
    string name, mac ;
    while(!inFile) {
        pair<string, pair<string, long>> namemac ;
        inFile >> namemac.second.second >> namemac.first ;
        if(namemac.first == "EOF") {
            break ;
        }
        nameMac.insert(namemac) ;
    }
    inFile.close() ;
}

//信号处理函数
void sighandle(int signo) {
    done = true ;
}

//接收消息
void recvMsg(RecvInfo ri, int sock) {
    
    while(!done) {
        unique_lock<mutex>lk(mute) ;
        while(notify || !ri.macIp.empty()) {
            cond.wait(lk) ;
        }

        ri.recvMsg(sock) ;
        notify = true ;
        cond.notify_one() ;
    }
}
