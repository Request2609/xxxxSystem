#pragma once
#include<iostream>
using namespace std ;
void printError(int line, string filename) {
    cout << "出错行：" << line << "文件名：" << filename << endl ;
    exit(1) ;
}

