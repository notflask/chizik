#pragma once

#include <cassert>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @class ClientSocket
 * @brief Represents an active client connection.
 * 
 * Uses RAII to ensure the socket is closed when the object goes out of scope.
 */
class ClientSocket {
  int m_fd;

public:
  /**
   * @brief Construct a new Client Socket object.
   * @param fd The file descriptor for the client connection.
   */
  explicit ClientSocket(int fd) : m_fd(fd) {}

  /**
   * @brief Destroy the Client Socket object and closes the connection.
   */
  ~ClientSocket() {
    if (m_fd >= 0)
      close(m_fd);
  };

  /**
   * @brief Reads a message from the client.
   * @return The message read as a string.
   */
  std::string receive() const;

  /**
   * @brief Sends a message to the client.
   * @param message The string message to send.
   */
  void send(const std::string &message) const;

  ClientSocket(const ClientSocket &) = delete;

  /**
   * @brief Move constructor for ClientSocket.
   */
  ClientSocket(ClientSocket &&other) noexcept : m_fd(other.m_fd) {
    other.m_fd = -1;
  }
};

/**
 * @brief A simple wrapper around POSIX sockets.
 *
 * Handles basic socket lifecycle: creation, binding, listening,
 * accepting connections, and basic I/O.
 */
class Socket {
private:
  int m_Sock;             ///< Server socket file descriptor
  sockaddr_in m_SockAddr; ///< Socket address configuration
public:
  /**
   * @brief Construct a new Socket object and binds to a port.
   * @param port The port number to listen on.
   */
  Socket(int port);

  /**
   * @brief Destroy the Socket object and closes open descriptors.
   */
  ~Socket();

  /**
   * @brief Blocks until a new client connection is accepted.
   */
  std::unique_ptr<ClientSocket> acceptConnection();
};
