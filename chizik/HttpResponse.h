#pragma once

#include <map>
#include <string>

struct HttpResponse {
    std::string version = "HTTP/1.1";
    int status_code = 200;
    std::string status_message = "OK";
    std::map<std::string, std::string> headers;
    std::string body;

    std::string to_string() const {
        std::string result = version + " " + std::to_string(status_code) + " " + status_message + "\r\n";

        for (const auto &header: headers) {
            result += header.first + ": " + header.second + "\r\n";
        }

        result += "\r\n";
        result += body;

        return result;
    }
};
