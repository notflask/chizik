#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "server/HttpServer.h"
#include "ContentType.h"

namespace fs = std::filesystem;

class FileServer {
private:
    HttpServer m_server;
    fs::path m_root_path;
    HttpServer::Handler m_file_not_found_handler = [](const HttpRequest&, HttpResponse& res) {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "<h1>404 - File not found</h1>";
        res.headers["Content-Type"] = "text/html";
    };

public:
  FileServer(int port, const fs::path &root_path = fs::current_path())
      : m_root_path(root_path), m_server(port) {
          m_server.get("/*", [this](const HttpRequest &req, HttpResponse &res) {
              std::string relative_path = req.path;

              if (relative_path == "/")
                  relative_path = "index.html";

              if (!relative_path.empty() && relative_path[0] == '/')
                  relative_path = relative_path.substr(1);

              fs::path path = m_root_path / relative_path;

              if (fs::exists(path) && fs::is_regular_file(path)) {
                  std::stringstream buffer;
                  std::ifstream file(path.c_str(), std::ios::binary);
                  buffer << file.rdbuf();

                  res.status_code = 200;
                  res.status_message = "OK";
                  res.body = buffer.str();

                  auto content_type = ContentTypeResolver::determine(path.string());
                  res.headers["Content-Type"] = ContentTypeResolver::toString(content_type);
              } else {
                  m_file_not_found_handler(req, res);
              }
          });
      }
  ~FileServer() = default;

  void run() { m_server.run(); }
  void notFound(HttpServer::Handler handler) {
      m_file_not_found_handler = handler;
      m_server.notFound(handler);
  }
};
