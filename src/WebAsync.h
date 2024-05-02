#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "ArduinoJson.h"
#include "ws2812b.h"

//void writeFile(fs::FS &fs, const char * path, const char * message);
//String readFile(fs::FS &fs, const char * path);
String processor(const String& var);
//void disp_show();
//void rgbled_color(int nled, uint8_t red,uint8_t green,uint8_t blue);
//void rgbled_show();

//extern sk sk_6812;
extern ws2812b ws;
//extern const char* PARAM_RGB_LED;
extern AsyncWebServer server;
//extern String ssid;
//extern String pass;
//extern const char* ssidPath;
//extern const char* passPath;
extern unsigned long sUpTime;
extern unsigned long ihour;
extern unsigned long imin;
extern unsigned long isec;
extern unsigned long iday;
//extern char hexChar[50];