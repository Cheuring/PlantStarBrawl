#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include <WinSock2.h>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <vector>

constexpr int PORT = 5099;

class MySocket {
public:
    MySocket();
    ~MySocket();

    void sendMsg(const std::string& msg);
    void recvMsg(std::string& buf);

    auto Connect(const char* str_ip, std::string& error_msg) -> bool;
    void Accept();

    auto GetLocalIP() -> std::string;



private:
    SOCKET sock, clntSock;
    std::string local_ip;

private:
    void trimstring(std::string& str);
    auto getlocalip() -> std::string;

};

#endif // _MY_SOCKET_H_