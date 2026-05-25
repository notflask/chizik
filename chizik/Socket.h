#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cassert>
#include <unistd.h>
#include <string>


class Socket {
private:
    int m_Sock;
    sockaddr_in m_SockAddr;
    int m_Client;
    int m_Result;
public:
    Socket(int port);
    ~Socket();

    void acceptConnection();
    void closeConnection();

    std::string receiveMessage() const;
    void sendMessage(const std::string& message);
};
