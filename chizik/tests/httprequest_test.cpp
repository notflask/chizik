#include <gtest/gtest.h>
#include "HttpRequest.h"

TEST(HttpRequestTest, ValidGetRequest) {
    std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    HttpRequest req;
    req.parse(raw);

    EXPECT_TRUE(req.isValid);
    EXPECT_EQ(req.method, "GET");
    EXPECT_EQ(req.path, "/index.html");
    EXPECT_EQ(req.version, "HTTP/1.1");
    EXPECT_EQ(req.headers["Host"], "localhost");
    EXPECT_EQ(req.body, "");
}

TEST(HttpRequestTest, ValidPostRequestWithBody) {
    std::string raw = "POST /login HTTP/1.1\r\nContent-Length: 10\r\n\r\ndata=hello";
    HttpRequest req;
    req.parse(raw);

    EXPECT_TRUE(req.isValid);
    EXPECT_EQ(req.method, "POST");
    EXPECT_EQ(req.body, "data=hello");
    EXPECT_EQ(req.headers["Content-Length"], "10");
}

TEST(HttpRequestTest, MissingCarriageReturn) {
    std::string raw = "GET / HTTP/1.1\nHost: localhost\n\n"; // Kein \r vorhanden!
    HttpRequest req;
    req.parse(raw);

    EXPECT_TRUE(req.isValid);
    EXPECT_EQ(req.method, "GET");
    EXPECT_EQ(req.headers["Host"], "localhost");
}

TEST(HttpRequestTest, InvalidGarbageData) {
    std::string raw = "Ich bin kein HTTP Request!";
    HttpRequest req;
    req.parse(raw);

    EXPECT_FALSE(req.isValid); // Das muss fehlschlagen!
}

TEST(HttpRequestTest, EmptyRequest) {
    std::string raw = "";
    HttpRequest req;
    req.parse(raw);

    EXPECT_FALSE(req.isValid);
}