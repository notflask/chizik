#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cassert>
#include <unistd.h>
#include <string>

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
    int m_Client;           ///< Current client connection file descriptor
    int m_Result;           ///< Result code of the last operation
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
    void acceptConnection();

    /**
     * @brief Closes the current client connection.
     */
    void closeConnection();

    /**
     * @brief Reads a message from the current client connection.
     * @return The message read as a string.
     */
    std::string receiveMessage() const;

    /**
     * @brief Sends a message to the current client connection.
     * @param message The string message to send.
     */
    void sendMessage(const std::string& message);
};
