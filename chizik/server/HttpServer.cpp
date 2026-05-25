#include "HttpServer.h"

void HttpServer::run() {
  while (true) {
    m_sock.acceptConnection();

    auto raw = m_sock.receiveMessage();

    if (raw.empty()) {
      m_sock.closeConnection();
      continue;
    }

    HttpRequest request;
    request.parse(raw);

    if (!request.isValid) {
      m_sock.closeConnection();
      continue;
    }

    HttpResponse response;

    Handler* handler = find_handler(request.method + " " + request.path);

    if (handler == nullptr) {
      m_not_found_handler(request, response);
    } else {
      try {
        (*handler)(request, response);
      } catch (const std::exception &e) {
        response.status_code = 500;
        response.body = "Internal Server Errror";
      }
    }

    response.headers["Content-Length"] = std::to_string(response.body.size());

    m_sock.sendMessage(response.to_string());
    m_sock.closeConnection();
  }
}

HttpServer::Handler *HttpServer::find_handler(const std::string &key) {
  auto it = m_routes.find(key);
  if (it != m_routes.end())
    return &it->second;

  for (auto &[route, handler] : m_routes) {
    if (route.ends_with("/*")) {
      std::string prefix = route.substr(0, route.size() - 1);
      if (key.starts_with(prefix))
        return &handler;
    }
  }
  return nullptr;
}
