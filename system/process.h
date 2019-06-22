#pragma once
#include"recvInfo.h"
#include"sendInfo.h"
#include<condition_variable>
#include<signal.h>
#include<fstream>
#include<mutex>
void cbFunc(RecvInfo ri, map<string, pair<string, long>>nameMac);
void sighandle(int signo) ;
void recvMsg(RecvInfo ri, int sock) ; 
void getNameMac(map<string, pair<string, long>>&nameMac) ;
