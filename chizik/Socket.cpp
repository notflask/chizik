#include "Socket.h"

Socket::Socket(const int port) : m_Client(0), m_Result(0) {
  m_Sock = socket(AF_INET, SOCK_STREAM, 0);
  m_SockAddr.sin_family = AF_INET;
  m_SockAddr.sin_addr.s_addr = INADDR_ANY;
  m_SockAddr.sin_port = htons(port);

  m_Result = bind(m_Sock, (struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
  assert(m_Result == 0 && "Failed to bind socket!");

  m_Result = listen(m_Sock, 5);
  assert(m_Result == 0 && "Error on listen()");
}

Socket::~Socket() {
  this->closeConnection();
  close(this->m_Sock);
}

void Socket::acceptConnection() {
  this->m_Client = accept(this->m_Sock, nullptr, nullptr);
}

std::string Socket::receiveMessage() const {
  char buffer[4096];
  int received = recv(this->m_Client, buffer, sizeof(buffer), 0);

  if (received == 0) {
    return "";
  }

  if (received > 0) {
    std::string msg(buffer, received);
    return msg;
  }

  return "";
}

void Socket::sendMessage(const std::string &message) {
  send(m_Client, message.c_str(), message.size(), 0);
}

void Socket::closeConnection() {
  if (this->m_Client != 0) {
    close(this->m_Client);
    this->m_Client = 0;
  }
}
