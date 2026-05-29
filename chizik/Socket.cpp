#include "Socket.h"
#include <cstring>
#include <iostream>
#include <sys/socket.h>

Socket::Socket(const int port) {
  m_Sock = socket(AF_INET, SOCK_STREAM, 0);

  if (m_Sock < 0) {
    std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    exit(1);
  }

  int opt = 1;
  setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

#ifdef __APPLE__
  setsockopt(m_Sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#endif

  m_SockAddr.sin_family = AF_INET;
  m_SockAddr.sin_addr.s_addr = INADDR_ANY;
  m_SockAddr.sin_port = htons(port);

  if (bind(m_Sock, (struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr)) < 0) {
    std::cerr << "Failed to bind to port " << port << ": " << strerror(errno)
              << std::endl;
    exit(1);
  }

  if (listen(m_Sock, 5) < 0) {
    std::cerr << "Error on listen(): " << strerror(errno) << std::endl;
    exit(1);
  }
}

Socket::~Socket() { close(this->m_Sock); }

std::unique_ptr<ClientSocket> Socket::acceptConnection() {
  int client_fd = accept(this->m_Sock, nullptr, nullptr);
  if (client_fd < 0)
    return nullptr;

  return std::make_unique<ClientSocket>(client_fd);
}

std::string ClientSocket::receive() const {
  char buffer[4096];
  int received = recv(this->m_fd, buffer, sizeof(buffer), 0);
  if (received <= 0)
    return "";
  return std::string(buffer, received);
}

void ClientSocket::send(const std::string &message) const {
  ::send(this->m_fd, message.c_str(), message.size(), 0);
}
