#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "ArduinoJson.h"

String processor(const String& var); //internal function

extern void writeFile(fs::FS &fs, const char * path, const char * message);
extern String readFile(fs::FS &fs, const char * path);

extern unsigned long sUpTime;
extern unsigned long ihour;
extern unsigned long imin;
extern unsigned long isec;
extern unsigned long iday;

extern void handleLogin(AsyncWebServerRequest *request);
extern void handleLogout(AsyncWebServerRequest *request);
extern void handleNotFound(AsyncWebServerRequest *request);
extern bool handleFileRead(AsyncWebServerRequest *request, String path);
extern bool is_authenticated(AsyncWebServerRequest *request);
