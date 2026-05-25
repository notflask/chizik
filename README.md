# chizik

A lightweight HTTP server library written in C++20. Built from scratch using POSIX sockets — no external dependencies.

## Features

- Route registration for GET and POST requests
- Wildcard routes (`/*`)
- Custom 404 handler
- Static file serving with automatic Content-Type detection
- Path traversal protection

## Requirements

- C++20
- CMake 3.15+
- POSIX-compatible OS (Linux, macOS)

## Building

```bash
./scripts/build.sh
```

For release build:

```bash
./scripts/build.sh --type Release
```

## Usage

### HttpServer

```cpp
#include "chizik/chizik.h"

int main() {
    HttpServer server(8080);

    server.get("/", [](const HttpRequest& req, HttpResponse& res) {
        res.status_code = 200;
        res.status_message = "OK";
        res.headers["Content-Type"] = "text/html";
        res.body = "<h1>Hello, World!</h1>";
    });

    server.notFound([](const HttpRequest& req, HttpResponse& res) {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "<h1>Page not found</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    server.run();
}
```

### FileServer

```cpp
#include "chizik/chizik.h"

int main() {
    FileServer server(8080, "./public");

    server.notFound([](const HttpRequest& req, HttpResponse& res) {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "<h1>File not found</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    server.run();
}
```

## Project Structure

```
chizik/          - Library source
  server/        - HttpServer and FileServer
  tests/         - Unit tests
examples/        - Usage examples
scripts/         - Build, run, and test scripts
```

## Running Tests

```bash
./scripts/test.sh
```

## License

MIT
