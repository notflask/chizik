# chizik

A lightweight HTTP server library written in C++20. Built from scratch using POSIX sockets with no external dependencies.

## Features

- Route registration for GET and POST requests
- Wildcard routes (e.g., `/api/v1/*`)
- Custom 404 handlers
- Static file serving with automatic Content-Type detection
- Path traversal protection

## Requirements

- C++20
- CMake 3.15+
- POSIX-compatible OS (Linux, macOS)

## Building and Running

The project includes a few scripts to make the development process easier.

### Running Examples

The `run.sh` script is the quickest way to see the server in action. It will automatically build the project before starting the executable.

To run the main HTTP server example:
```bash
./scripts/run.sh http_server_example
```

To run the file server example:
```bash
# First, create a public directory with an index file
mkdir -p public
echo "<h1>Hello from the file server</h1>" > public/index.html

# Then run the example
./scripts/run.sh file_server_example
```

### Manual Builds

If you just want to compile without running, use the build script:
```bash
./scripts/build.sh
```

You can specify the build type and target if needed:
```bash
./scripts/build.sh --type Release http_server_example
```

## Development Examples

The code for these examples is located in the `examples/` directory.

### Basic HTTP Server

The `http_server_example` demonstrates how to set up routes, handle POST data, and return JSON.

```cpp
#include "chizik/chizik.h"
#include <iostream>

int main() {
    HttpServer server(8080);

    // Simple HTML route
    server.get("/", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "<h1>Welcome</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    // API route with wildcard
    server.get("/api/*", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "{\"status\": \"ok\"}";
        res.headers["Content-Type"] = "application/json";
    });

    std::cout << "Running on http://localhost:8080" << std::endl;
    server.run();
}
```

### Static File Server

The `file_server_example` is built on top of `HttpServer` and handles serving files from a local directory.

```cpp
#include "chizik/chizik.h"

int main() {
    // Serves files from the "public" directory
    FileServer server(8080, "public");
    server.run();
}
```

## Running Tests

To run the unit tests for the core components:
```bash
./scripts/test.sh
```

## Project Structure

- `chizik/`: Core library source.
- `chizik/server/`: HttpServer and FileServer implementations.
- `chizik/tests/`: Unit tests using GoogleTest.
- `examples/`: Ready-to-run demonstration files.
- `scripts/`: Utility scripts for building, running, and testing.

## License

MIT
