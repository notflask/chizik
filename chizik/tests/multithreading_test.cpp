#include "../server/HttpServer.h"
#include <arpa/inet.h>
#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

TEST(HttpServerTest, ConcurrentRequests) {
  const int port = 56789;
  HttpServer server(port);

  server.get("/slow", [](const HttpRequest &, HttpResponse &res) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    res.body = "Done";
  });

  std::atomic<bool> running{true};
  std::thread serverThread([&server, &running]() { server.run(); });
  serverThread.detach();

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  auto start = std::chrono::steady_clock::now();

  const int numClients = 5;
  std::vector<std::thread> clients;
  std::atomic<int> successCount{0};

  for (int i = 0; i < numClients; ++i) {
    clients.emplace_back([port, &successCount]() {
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

      if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        std::string req = "GET /slow HTTP/1.1\r\nHost: localhost\r\n\r\n";
        send(sock, req.c_str(), req.size(), 0);

        char buffer[1024];
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes > 0)
          successCount++;
      }
      close(sock);
    });
  }

  for (auto &t : clients)
    t.join();

  auto end = std::chrono::steady_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  EXPECT_EQ(successCount, numClients);

  EXPECT_LT(duration, 600);
}
