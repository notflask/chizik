/**
 * @file FileServer.h
 * @brief Simple static file server implementation.
 */

#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

#include "ContentType.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpServer.h"

namespace fs = std::filesystem;

/**
 * @class FileServer
 * @brief A server that serves static files from a root directory.
 *
 * This class uses an underlying HttpServer to handle HTTP requests and
 * map them to files on the local filesystem.
 */
class FileServer {
private:
  /// The underlying HTTP server.
  HttpServer m_server;
  /// The root directory from which files are served.
  fs::path m_root_path;
  /// Default 404 handler.
  HttpServer::Handler m_file_not_found_handler = [](const HttpRequest &,
                                                    HttpResponse &res) {
    res.status_code = 404;
    res.status_message = "Not Found";
    res.body = "<h1>404 - File not found</h1>";
    res.headers["Content-Type"] = "text/html";
  };

public:
  /**
   * @brief Constructs a new FileServer.
   *
   * @param port The port to listen on.
   * @param root_path The root directory to serve files from (defaults to current directory).
   */
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
      if (canonical.string().find(fs::weakly_canonical(m_root_path).string()) !=
          0) {
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
        res.headers["Content-Type"] =
            ContentTypeResolver::toString(content_type);
      } else {
        m_file_not_found_handler(req, res);
      }
    });
  }

  /**
   * @brief Destructor for FileServer.
   */
  ~FileServer() = default;

  /**
   * @brief Starts the file server and begins listening for requests.
   *
   * This call is blocking.
   */
  void run() { m_server.run(); }

  /**
   * @brief Sets a custom handler for 404 Not Found errors.
   *
   * @param handler The handler function to be called when a file is not found.
   */
  void notFound(HttpServer::Handler handler) {
    m_file_not_found_handler = handler;
    m_server.notFound(handler);
  }
};
