#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <exception>
#include <thread>
#include <utility>

void HttpServer::run() {
  while (true) {
    auto client = m_sock.acceptConnection();
    if (!client)
      continue;

    std::jthread([this, c = std::move(client)]() mutable {
      auto raw = c->receive();
      if (raw.empty())
        return;

      HttpRequest request;
      request.parse(raw);
      if (!request.isValid)
        return;

      HttpResponse response;
      Handler *handler = find_handler(request.method + " " + request.path);

      if (handler == nullptr) {
        m_not_found_handler(request, response);
      } else {
        try {
          (*handler)(request, response);
        } catch (const std::exception &e) {
          response.status_code = 500;
          response.body = "Internal Server Error";
        }
      }

      response.headers["Content-Length"] = std::to_string(response.body.size());

      c->send(response.to_string());
    }).detach();
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
