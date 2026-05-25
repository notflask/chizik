#pragma once

#include <string>

enum class ContentType {
    Html,
    Css,
    Javascript,
    Jpeg,
    Png,
    PlainText
};

class ContentTypeResolver {
public:
    static ContentType determine(const std::string& path) {
        if (path.find(".html") != std::string::npos) return ContentType::Html;
        if (path.find(".css")  != std::string::npos) return ContentType::Css;
        if (path.find(".js")   != std::string::npos) return ContentType::Javascript;
        if (path.find(".jpg")  != std::string::npos ||
            path.find(".jpeg") != std::string::npos) return ContentType::Jpeg;
        if (path.find(".png")  != std::string::npos) return ContentType::Png;

        return ContentType::PlainText;
    }

    static std::string toString(ContentType type) {
        switch (type) {
            case ContentType::Html:       return "text/html";
            case ContentType::Css:        return "text/css";
            case ContentType::Javascript: return "application/javascript";
            case ContentType::Jpeg:       return "image/jpeg";
            case ContentType::Png:        return "image/png";
            case ContentType::PlainText:  return "text/plain";
            default:                      return "text/plain";
        }
    }
};
