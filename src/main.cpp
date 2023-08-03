#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Ticker.h>
#include "sk.h"

//-----------------
#include "main.h"   //определяет функции во внешних файлах
//-----------------


//Управдение внутренним RGB светодиодом
#define GPIO_RGB_BUILTIN_LED 21

sk sk_6812; //class sk create !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Search for parameter in HTTP POST request
const char* PARAM_RGB_LED = "rgbled";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//RSSI as TEXT
String getRSSI() {
  long rssi = WiFi.RSSI();
  return String(rssi);//преобразование типов
}

void setup() {

   Serial.begin(115200);

  //GRB LED
  sk_6812.begin( GPIO_RGB_BUILTIN_LED, 1);  //init RGB LED class 
  sk_6812.color(0,2,1,3,0); //GRBW
  sk_6812.show();

  Serial.println("-----------------------------------------");
  //Serial.println("Serial init speed 115200..");
  Serial.printf("Chip Model = %s Rev = %d \r\n", ESP.getChipModel(),ESP.getChipRevision());
  Serial.printf("Chip Cores = %d \r\n", ESP.getChipCores());
  //Serial.println("ChipID = ");Serial.println(chipid);
  Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
  Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
  Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
  Serial.println("-----------------------------------------");

  initSPIFFS(); //инициализацич SPIFFS

  String ssid = "tenda"; //WIFI SSID
  String pass = "tenda_639122"; //WIFI PASS
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
Serial.print("IP address: ");Serial.println(WiFi.localIP());

  // Route for root / web page
  web_init();
  server.begin();

}



void loop() {

    delay(1000);

}
