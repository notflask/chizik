#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/**
 * @brief Simple benchmark tool for the Http Server.
 * 
 * Sends a large number of requests concurrently and measures performance.
 */

const std::string REQUEST = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";

void send_requests(int port, int requests_per_thread, std::atomic<int>& success_count, std::atomic<long long>& total_latency_ms) {
    for (int i = 0; i < requests_per_thread; ++i) {
        auto start = std::chrono::steady_clock::now();
        
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) continue;

        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
            send(sock, REQUEST.c_str(), REQUEST.size(), 0);
            
            char buffer[1024];
            int bytes = recv(sock, buffer, sizeof(buffer), 0);
            
            if (bytes > 0) {
                auto end = std::chrono::steady_clock::now();
                total_latency_ms += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                success_count++;
            }
        }
        close(sock);
    }
}

int main(int argc, char* argv[]) {
    int port = 8080;
    int num_threads = 10;
    int requests_per_thread = 100;

    if (argc > 1) port = std::stoi(argv[1]);
    if (argc > 2) num_threads = std::stoi(argv[2]);
    if (argc > 3) requests_per_thread = std::stoi(argv[3]);

    int total_requests = num_threads * requests_per_thread;

    std::cout << "Starting benchmark on port " << port << "..." << std::endl;
    std::cout << "Threads: " << num_threads << ", Requests per thread: " << requests_per_thread << std::endl;
    std::cout << "Total requests: " << total_requests << std::endl;

    std::atomic<int> success_count{0};
    std::atomic<long long> total_latency_ms{0};
    
    auto start_time = std::chrono::steady_clock::now();

    std::vector<std::thread> workers;
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back(send_requests, port, requests_per_thread, std::ref(success_count), std::ref(total_latency_ms));
    }

    for (auto& t : workers) {
        t.join();
    }

    auto end_time = std::chrono::steady_clock::now();
    auto total_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    double seconds = total_duration_ms / 1000.0;
    double rps = success_count / seconds;
    double avg_latency = success_count > 0 ? (double)total_latency_ms / success_count : 0;

    std::cout << "\n--- Benchmark Results ---" << std::endl;
    std::cout << "Successful requests: " << success_count << "/" << total_requests << std::endl;
    std::cout << "Total time:          " << seconds << " s" << std::endl;
    std::cout << "Requests per second: " << rps << std::endl;
    std::cout << "Avg Latency:         " << avg_latency << " ms" << std::endl;

    return 0;
}
