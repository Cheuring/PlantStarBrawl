#include "MySocket.h"

MySocket::MySocket(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        throw std::runtime_error("WSAStartup failed!");
    }

    local_ip = getlocalip();
}

MySocket::~MySocket(){
    closesocket(clntSock);
    closesocket(sock);
    WSACleanup();
}

void MySocket::sendMsg(const std::string& msg){
    int iSend = send(clntSock, msg.c_str(), msg.size(), 0);
    if(iSend == SOCKET_ERROR){
        throw std::runtime_error("send() failed!");
    }
}

void MySocket::recvMsg(std::string& buf){
    char recvBuf[1024];
    memset(recvBuf, 0, sizeof(recvBuf));

    int recvLen = recv(clntSock, recvBuf, sizeof(recvBuf), 0);
    if(recvLen <= 0){
        throw std::runtime_error("recv() failed!");
    }

    buf = recvBuf;
}

auto MySocket::Connect(const char* str_ip, std::string& error_msg) -> bool {
    try {
        SOCKADDR_IN sockAddr;
        sockAddr.sin_family = PF_INET;
        sockAddr.sin_port = htons(PORT);
        sockAddr.sin_addr.s_addr = inet_addr(str_ip);

        clntSock = socket(PF_INET, SOCK_STREAM, 0);
        if (clntSock == INVALID_SOCKET) {
            throw std::runtime_error("socket() failed!");
        }

        if (connect(clntSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
            throw std::runtime_error("connect() failed!");
        }

        // 连接成功，退出循环
        // std::cout << "Connected to server: " << ip << std::endl;
        // break;
        return true;
    } catch (const std::exception& e) {
        // std::cerr << e.what() << std::endl;
        // std::cerr << "Please try again." << std::endl;
        error_msg = e.what();
        closesocket(clntSock);
        return false;
    }
}

void MySocket::Accept() {
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        WSACleanup();
        throw std::runtime_error("socket() failed!");
    }

    SOCKADDR_IN sockAddr;
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_port = htons(PORT);
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int retVal = bind(sock, (LPSOCKADDR)&sockAddr, sizeof(SOCKADDR_IN));
    if(retVal == SOCKET_ERROR){
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("bind() failed!");
    }

    // std::cout << "Server ip: " << getlocalip() << "\nWating for player\n" << std::endl;

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
}

auto MySocket::GetLocalIP() -> std::string {
    return local_ip;
}

void MySocket::trimstring(std::string& str) {
    if (!str.empty())
    {
        str.erase(0, str.find_first_not_of(" "));
        str.erase(str.find_last_not_of(" ") + 1);
    }
}

auto MySocket::getlocalip() -> std::string {
    std::string ip("127.0.0.1");
    std::string ipconfig_content;

    FILE* fp = _popen("ipconfig", "r");
    if (NULL != fp)
    {
        char line[4096];
        while (NULL != fgets(line, sizeof(line), fp))
        {
            ipconfig_content += line;
        }

        auto p = ipconfig_content.find("IPv4");
        if (p != std::string::npos)
        {
            auto p2 = ipconfig_content.find(":", p);
            if (p2 != std::string::npos)
            {
                auto p3 = ipconfig_content.find("\n", p2);
                if (p3 != std::string::npos)
                {
                    ip = ipconfig_content.substr(p2 + 1, p3 - p2 - 1);
                    trimstring(ip);
                }
            }
        }

        _pclose(fp);
    }

    return ip;
}