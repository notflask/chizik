/**
 * @file ContentType.h
 * @brief Utilities for handling HTTP Content-Type headers.
 */

#pragma once

#include <string>

/**
 * @brief Supported HTTP Content-Types.
 */
enum class ContentType {
  Html,       ///< text/html
  Css,        ///< text/css
  Javascript, ///< application/javascript
  Svg,        ///< image/svg+xml
  Ico,        ///< image/x-icon
  Json,       ///< application/json
  Gif,        ///< image/gif
  Woff,       ///< font/woff
  Woff2,      ///< font/woff2
  Jpeg,       ///< image/jpeg
  Png,        ///< image/png
  PlainText   ///< text/plain
};

/**
 * @brief Helper class to resolve file paths to ContentTypes and strings.
 */
class ContentTypeResolver {
public:
  /**
   * @brief Determines the ContentType based on a file path's extension.
   * @param path The file path or name.
   * @return ContentType The detected content type.
   */
  static ContentType determine(const std::string &path) {
    if (path.find(".html") != std::string::npos)
      return ContentType::Html;
    if (path.find(".css") != std::string::npos)
      return ContentType::Css;
    if (path.find(".js") != std::string::npos)
      return ContentType::Javascript;
    if (path.find(".jpg") != std::string::npos ||
        path.find(".jpeg") != std::string::npos)
      return ContentType::Jpeg;
    if (path.find(".png") != std::string::npos)
      return ContentType::Png;
    if (path.find(".svg") != std::string::npos)
      return ContentType::Svg;
    if (path.find(".ico") != std::string::npos)
      return ContentType::Ico;
    if (path.find(".json") != std::string::npos)
      return ContentType::Json;
    if (path.find(".gif") != std::string::npos)
      return ContentType::Gif;
    if (path.find(".woff") != std::string::npos)
      return ContentType::Woff;
    if (path.find(".woff2") != std::string::npos)
      return ContentType::Woff2;

    return ContentType::PlainText;
  }

  /**
   * @brief Converts a ContentType enum to its corresponding MIME type string.
   * @param type The ContentType to convert.
   * @return std::string The MIME type string (e.g., "text/html").
   */
  static std::string toString(ContentType type) {
    switch (type) {
    case ContentType::Html:
      return "text/html";
    case ContentType::Css:
      return "text/css";
    case ContentType::Javascript:
      return "application/javascript";
    case ContentType::Jpeg:
      return "image/jpeg";
    case ContentType::Png:
      return "image/png";
    case ContentType::PlainText:
      return "text/plain";
    case ContentType::Svg:
      return "image/svg+xml";
    case ContentType::Ico:
      return "image/x-icon";
    case ContentType::Json:
      return "application/json";
    case ContentType::Gif:
      return "image/gif";
    case ContentType::Woff:
      return "font/woff";
    case ContentType::Woff2:
      return "font/woff2";
    default:
      return "text/plain";
    }
  }
};
