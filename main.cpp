#include "../chizik/chizik.h"
#include <filesystem>
#include <iostream>

int main() {
    FileServer server(8080, fs::current_path() / "build/debug/public");

    server.notFound([](const HttpRequest& req, HttpResponse& res) {
        res.status_code = 404;
        res.status_message = "Not Found";
        res.body = "<h1>Meine custom 404 Seite</h1>";
        res.headers["Content-Type"] = "text/html";
    });

    std::cout << "FileServer running on http://localhost:8080\n";
    server.run();
    return 0;
}
