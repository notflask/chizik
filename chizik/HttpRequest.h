#pragma once

#include <map>
#include <sstream>
#include <string>

/**
 * @brief Represents an incoming HTTP request.
 * 
 * This structure holds the parsed data of an HTTP request, including
 * method, path, version, headers, and body.
 */
struct HttpRequest {
    std::string method;  ///< HTTP method (e.g., GET, POST)
    std::string path;    ///< Requested URI path
    std::string version; ///< HTTP version (e.g., HTTP/1.1)

    std::map<std::string, std::string> headers; ///< HTTP headers

    std::string body; ///< Request body content

    bool isValid = false; ///< Flag indicating if the request was parsed successfully

    /**
     * @brief Converts the request back to a raw string format.
     * @return A string representation of the HTTP request.
     */
    std::string to_string() const {
        std::string result = method + " " + path + " " + version + "\r\n";

        for (const auto &header: headers) {
            result += header.first + ": " + header.second + "\r\n";
        }

        result += "\r\n";
        result += body;

        return result;
    }

    /**
     * @brief Parses a raw HTTP request string.
     * 
     * Extracts method, path, version, headers, and body from the raw input.
     * Sets the isValid flag to true if parsing is successful.
     * 
     * @param raw The raw HTTP request string to parse.
     */
    void parse(const std::string &raw) {
        isValid = false;

        if (raw.empty())
            return;

        std::stringstream ss(raw);
        std::string line;

        if (std::getline(ss, line)) {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            std::istringstream ls(line);
            ls >> method >> path >> version;

            if (method.empty() || path.empty() || version.empty())
                return;

            if (version.find("HTTP/") != 0)
                return;
        }
        else {
            return;
        }

        while (getline(ss, line)) {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            if (line.empty())
                break;

            size_t colon_pos = line.find(':');
            if (colon_pos == std::string::npos)
                continue;

            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);

            size_t start_pos = value.find_first_not_of(" ");
            if (start_pos != std::string::npos)
                value = value.substr(start_pos);

            headers[key] = value;
        }

        std::ostringstream bss;
        bss << ss.rdbuf();
        body = bss.str();

        isValid = true;
    }
};
