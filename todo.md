# chizik — TODO

## 🐛 Bugs / Korrektheit

- [x] `Socket.cpp` — `m_Result` wird im Konstruktor nicht initialisiert, aber als Member deklariert
- [x] `Socket::receiveMessage()` — trimmt Whitespace vom Ende der Nachricht, was bei Binärdaten (Bilder, etc.) Daten kaputt macht
- [x] `HttpServer.cpp` — `"Internal Server Errror"` hat einen Tippfehler (3x r)
- [x] `examples/file_server.cpp` — `fs::` wird benutzt aber `<filesystem>` wird nicht included (kommt nur transitiv rein)

## ✨ Fehlende Features

- [ ] `HttpServer` — keine `put()` und `delete_()` Methoden
- [ ] `HttpServer` — Multithreading fehlt, aktuell nur ein Request gleichzeitig möglich
- [ ] `ContentType` — fehlt `svg`, `ico`, `json`, `gif`, `woff`, `woff2`
- [x] `FileServer` — kein Schutz gegen Path Traversal (`../../etc/passwd`)

## 🧹 Code-Qualität

- [x] `chizik/server/FileServer.h` — `#include "server/HttpServer.h"` sollte `#include "HttpServer.h"` sein
- [x] `HttpServer` — `find_handler` ist `public` nur für Tests — besser `private` lassen
- [x] `chizik/CMakeLists.txt` — `examples/` Ordner ist nicht als Target registriert

## 📖 Dokumentation

- [ ] `README.md` fehlt komplett
- [x] `FileServer.h` — hat keine Doxygen-Kommentare
