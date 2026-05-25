add_library(chizik
    Socket.cpp
    Socket.h
    HttpRequest.h
    HttpResponse.h
    server/FileServer.h
    server/HttpServer.cpp
    server/HttpServer.h
    ContentType.h
)

target_include_directories(chizik PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)
