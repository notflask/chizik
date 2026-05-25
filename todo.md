# chizik — TODO

## 🐛 Bugs / Korrektheit

- [ ] `Socket.cpp` — `m_Result` wird im Konstruktor nicht initialisiert, aber als Member deklariert
- [ ] `Socket::receiveMessage()` — trimmt Whitespace vom Ende der Nachricht, was bei Binärdaten (Bilder, etc.) Daten kaputt macht
- [ ] `HttpServer.cpp` — `"Internal Server Errror"` hat einen Tippfehler (3x r)
- [ ] `examples/file_server.cpp` — `fs::` wird benutzt aber `<filesystem>` wird nicht included (kommt nur transitiv rein)

## ✨ Fehlende Features

- [ ] `HttpServer` — keine `put()` und `delete_()` Methoden
- [ ] `HttpServer` — Multithreading fehlt, aktuell nur ein Request gleichzeitig möglich
- [ ] `ContentType` — fehlt `svg`, `ico`, `json`, `gif`, `woff`, `woff2`
- [ ] `FileServer` — kein Schutz gegen Path Traversal (`../../etc/passwd`)

## 🧹 Code-Qualität

- [ ] `chizik/server/FileServer.h` — `#include "server/HttpServer.h"` sollte `#include "HttpServer.h"` sein
- [ ] `HttpServer` — `find_handler` ist `public` nur für Tests — besser `private` lassen
- [ ] `chizik/CMakeLists.txt` — `examples/` Ordner ist nicht als Target registriert

## 📖 Dokumentation

- [ ] `README.md` fehlt komplett
- [ ] `FileServer.h` — hat keine Doxygen-Kommentare
