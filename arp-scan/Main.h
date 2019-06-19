#pragma once
#include<string>
#include<iostream>
#include<map>
#include<fstream>
using namespace std ;
class Main
{
public:
    Main() {}
    ~Main() {}

private:

};
class getIp {
public :
    void getMac() ;
private :
    map<string, string> info_list; 
} ;

//从文件中获取mac地址和姓名
void getIp :: getMac() {
    
    ifstream in("map.txt", ios::in) ;
    if(!in) {
        cout << "文件打不开！" << endl ;
    }
    while(!in.eof()) {
        pair<string, string> p ;
        cout << p.first  << p.second <<endl ;
        info_list.insert(p) ;
    }
    cout << info_list.size() << endl ;
    in.close() ;
}
