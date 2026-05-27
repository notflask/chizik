#pragma once

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Socket.h"
#include <functional>
#include <string>
#include <unordered_map>

/**
 * @brief Core HTTP Server class for routing and handling requests.
 *
 * Allows registering handlers for different HTTP methods and paths.
 */
class HttpServer {
public:
  /**
   * @brief Type definition for request handlers.
   * Receives a request and modifies a response object.
   */
  using Handler = std::function<void(const HttpRequest &, HttpResponse &)>;

  /**
   * @brief Construct a new Http Server object.
   * @param port The port to listen on.
   */
  HttpServer(int port) : m_sock(port), m_routes({}) {}

  /**
   * @brief Starts the server loop to accept and process connections.
   */
  void run();

  /**
   * @brief Registers a GET request handler for a specific path.
   * @param path The URI path to handle.
   * @param handler The function to execute.
   */
  void get(const std::string &path, Handler handler) {
    m_routes["GET " + path] = handler;
  }

  /**
   * @brief Registers a POST request handler for a specific path.
   * @param path The URI path to handle.
   * @param handler The function to execute.
   */
  void post(const std::string &path, Handler handler) {
    m_routes["POST " + path] = handler;
  }

  /**
   * @brief Customizes the handler for 404 Not Found responses.
   * @param handler The function to execute for unknown routes.
   */
  void notFound(Handler handler) { m_not_found_handler = handler; }

private:
  Socket m_sock; ///< Underlying socket for network I/O
  std::unordered_map<std::string, Handler> m_routes; ///< Registered route map

  /**
   * @brief Internal helper to find a handler for a given routing key.
   * @param key The combined method and path (e.g., "GET /index").
   * @return Handler* Pointer to the handler if found, nullptr otherwise.
   */
  Handler *find_handler(const std::string &key);

  /**
   * @brief Default handler for 404 Not Found.
   */
  Handler m_not_found_handler = [](const HttpRequest &, HttpResponse &res) {
    res.status_code = 404;
    res.status_message = "Not Found";
    res.body = "<h1>404 - Not Found</h1>";
    res.headers["Content-Type"] = "text/html";
  };
};
