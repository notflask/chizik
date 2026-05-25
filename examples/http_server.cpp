#include "../chizik/chizik.h"
#include <iostream>

int main() {
    HttpServer server(8080);

    // 1. Static HTML Route
    server.get("/", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "<h1>Chizik Multi-Feature Example</h1>"
                   "<ul>"
                   "<li><a href='/hello'>Simple GET</a></li>"
                   "<li><a href='/api/v1/status'>JSON API</a></li>"
                   "<li><a href='/post-test'>POST Test Form</a></li>"
                   "</ul>";
        res.headers["Content-Type"] = "text/html";
    });

    server.get("/hello", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "<h1>Hello from Chizik!</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    // 2. JSON API with Wildcard
    server.get("/api/v1/*", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "{\"status\": \"ok\", \"requested_path\": \"" + req.path + "\"}";
        res.headers["Content-Type"] = "application/json";
    });

    // 3. POST Handling
    server.get("/post-test", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "<html><body>"
                   "<h2>POST Tester</h2>"
                   "<form method='POST' action='/submit'>"
                   "<input type='text' name='data' placeholder='Type something...'>"
                   "<button type='submit'>Submit</button>"
                   "</form>"
                   "</body></html>";
        res.headers["Content-Type"] = "text/html";
    });

    server.post("/submit", [](const HttpRequest& req, HttpResponse& res) {
        res.body = "Server received POST data: " + req.body;
        res.headers["Content-Type"] = "text/plain";
    });

    // 4. Custom 404 Handler
    server.notFound([](const HttpRequest& req, HttpResponse& res) {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "<h1>Custom 404: Page '" + req.path + "' not found</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    std::cout << "HttpServer running on http://localhost:8080\n";
    server.run();

    return 0;
}
