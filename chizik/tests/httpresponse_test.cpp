#include <gtest/gtest.h>
#include "HttpResponse.h"

TEST(HttpResponseTest, Valid200Response) {
    HttpResponse res;
    res.version = "HTTP/1.1";
    res.status_code = 200;
    res.status_message = "OK";

    res.headers["Content-Type"] = "text/html";
    res.headers["Content-Length"] = "20";

    res.body = "<h1>Hallo Welt</h1>";

    std::string expected =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 20\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<h1>Hallo Welt</h1>";

    EXPECT_EQ(res.to_string(), expected);
}

TEST(HttpResponseTest, Valid404ResponseWithoutBody) {
    HttpResponse res;
    res.version = "HTTP/1.1";
    res.status_code = 404;
    res.status_message = "Not Found";

    std::string expected =
        "HTTP/1.1 404 Not Found\r\n"
        "\r\n";

    EXPECT_EQ(res.to_string(), expected);
}

TEST(HttpResponseTest, Valid500ResponseWithCustomHeader) {
    HttpResponse res;
    res.version = "HTTP/1.1";
    res.status_code = 500;
    res.status_message = "Internal Server Error";

    res.headers["Connection"] = "close";
    res.headers["X-Server-Name"] = "MyCppServer";

    std::string expected =
        "HTTP/1.1 500 Internal Server Error\r\n"
        "Connection: close\r\n"
        "X-Server-Name: MyCppServer\r\n"
        "\r\n";

    EXPECT_EQ(res.to_string(), expected);
}