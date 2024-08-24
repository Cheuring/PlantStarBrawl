#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include <WinSock2.h>
#include <cstdio>
#include <stdexcept>

constexpr int PORT = 5099;
#pragma comment(lib, "ws2_32.lib")

class MySocket {
public:
    MySocket(bool is_server){
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
            throw std::runtime_error("WSAStartup failed!");
        }

        if(is_server){
            sock = socket(PF_INET, SOCK_STREAM, 0);
            if(sock == INVALID_SOCKET){
                throw std::runtime_error("socket() failed!");
            }

            SOCKADDR_IN sockAddr;
            sockAddr.sin_family = PF_INET;
            sockAddr.sin_port = htons(PORT);
            sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

            int retVal = bind(sock, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR_IN));
            if(retVal == SOCKET_ERROR){
                throw std::runtime_error("bind() failed!");
            }

            if(listen(sock, 5) == SOCKET_ERROR){
                throw std::runtime_error("listen() failed!");
            }

            SOCKADDR_IN clntAddr;
            int len = sizeof(SOCKADDR);

            clntSock = accept(sock, (SOCKADDR*)&clntAddr, &len);
            if(clntSock == INVALID_SOCKET){
                throw std::runtime_error("accept() failed!");
            }

            printf("Accept client IP: %s\n", inet_ntoa(clntAddr.sin_addr));
        }else{
            SOCKADDR_IN sockAddr;
            sockAddr.sin_family = PF_INET;
            sockAddr.sin_port = htons(PORT);
            sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
            
            clntSock = socket(PF_INET, SOCK_STREAM, 0);
            if(clntSock == INVALID_SOCKET){
                throw std::runtime_error("socket() failed!");
            }

            if(connect(clntSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR){
                throw std::runtime_error("connect() failed!");
            }
        }
    }
    ~MySocket(){
        closesocket(clntSock);
        closesocket(sock);
        WSACleanup();
    }

    void sendMsg(const std::string& msg){
        int iSend = send(clntSock, msg.c_str(), msg.size(), 0);
        if(iSend == SOCKET_ERROR){
            throw std::runtime_error("send() failed!");
        }
    }

    void recvMsg(std::string& buf){
        char recvBuf[1024];
        memset(recvBuf, 0, sizeof(recvBuf));

        int recvLen = recv(clntSock, recvBuf, sizeof(recvBuf), 0);
        if(recvLen <= 0){
            throw std::runtime_error("recv() failed!");
        }

        buf = recvBuf;
    }

private:
    SOCKET sock, clntSock;
};


#endif // _MY_SOCKET_H_