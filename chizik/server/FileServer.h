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

              // Path Traversal Fix
              fs::path canonical = fs::weakly_canonical(path);
              if (canonical.string().find(fs::weakly_canonical(m_root_path).string()) != 0) {
                  m_file_not_found_handler(req, res);
                  return;
              }

              if (fs::exists(canonical) && fs::is_regular_file(canonical)) {
                  std::ifstream file(canonical, std::ios::binary);
                  std::stringstream buffer;
                  buffer << file.rdbuf();
                  res.status_code = 200;
                  res.status_message = "OK";
                  res.body = buffer.str();
                  auto content_type = ContentTypeResolver::determine(canonical.string());
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
