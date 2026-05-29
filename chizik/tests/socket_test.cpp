#include "Socket.h"
#include <arpa/inet.h>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <unistd.h>

TEST(SocketTest, Initialization) {
  EXPECT_NO_THROW({ Socket testSocket(0); });
}

TEST(SocketTest, AcceptAndReceiveMessage) {
  const int testPort = 54321;

  std::thread serverThread([testPort]() {
    Socket server(testPort);

    auto client = server.acceptConnection();
    ASSERT_NE(client, nullptr);

    std::string msg = client->receive();

    EXPECT_EQ(msg, "Hallo Server!");

    client->send("Hallo Client!");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  int clientSock = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(testPort);
  inet_pton(AF_INET, "127.0.0.1",
            &serverAddr.sin_addr); // 127.0.0.1 ist localhost

  int res =
      connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  ASSERT_EQ(res, 0) << "Der Client konnte sich nicht mit dem Server verbinden!";

  std::string testMsg = "Hallo Server!";
  send(clientSock, testMsg.c_str(), testMsg.size(), 0);

  close(clientSock);

  serverThread.join();
}
