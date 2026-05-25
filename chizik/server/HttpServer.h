#pragma once

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Socket.h"
#include <unordered_map>
#include <functional>
#include <string>

class HttpServer {
public:
  using Handler = std::function<void(const HttpRequest &, HttpResponse &)>;
  HttpServer(int port) : m_sock(port), m_routes({}) {}

  void run();

  void get(const std::string &path, Handler handler) {
    m_routes["GET " + path] = handler;
  }

  void post(const std::string &path, Handler handler) {
    m_routes["POST " + path] = handler;
  }

  void notFound(Handler handler) { m_not_found_handler = handler; }

  Handler *find_handler(const std::string &key);

private:
  Socket m_sock;
  std::unordered_map<std::string, Handler> m_routes;

  Handler m_not_found_handler = [](const HttpRequest &, HttpResponse &res) {
    res.status_code = 404;
    res.status_message = "Not Found";
    res.body = "<h1>404 - Not Found</h1>";
    res.headers["Content-Type"] = "text/html";
  };
};
