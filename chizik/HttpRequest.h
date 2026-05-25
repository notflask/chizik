#pragma once

#include <map>
#include <sstream>
#include <string>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;

    std::map<std::string, std::string> headers;

    std::string body;

    bool isValid = false;

    std::string to_string() const {
        std::string result = method + " " + path + " " + version + "\r\n";

        for (const auto &header: headers) {
            result += header.first + ": " + header.second + "\r\n";
        }

        result += "\r\n";
        result += body;

        return result;
    }

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
