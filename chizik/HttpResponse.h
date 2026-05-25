#pragma once

#include <map>
#include <string>

/**
 * @brief Represents an HTTP response to be sent to a client.
 */
struct HttpResponse {
    std::string version = "HTTP/1.1"; ///< HTTP version (defaults to HTTP/1.1)
    int status_code = 200;            ///< HTTP status code (e.g., 200, 404)
    std::string status_message = "OK"; ///< Status message (e.g., OK, Not Found)
    std::map<std::string, std::string> headers; ///< HTTP response headers
    std::string body;                 ///< Response body content

    /**
     * @brief Serializes the response into a raw string for network transmission.
     * @return Raw HTTP response string.
     */
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
