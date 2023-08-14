#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include "SPIFFS.h"
#include <Ticker.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//#include <SPI.h>
#include <U8g2lib.h>
//#include "sk.h"
#include "ws2812b.h"

//-----------------
#include "main.h"   //определяет функции во внешних файлах
//-----------------

#define NTP_OFFSET  60 * 60 * 3 // In seconds +3h
#define NTP_INTERVAL 5 * 60 * 1000    // In miliseconds
//#define NTP_ADDRESS  "1.asia.pool.ntp.org"
#define NTP_ADDRESS  "1.ua.pool.ntp.org"

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


//Управдение RGB светодиодной лентой
#define GPIO_RGB_BUILTIN_LED 7

//SPI for SH1106 OLED 128x64
#define SPI_CLOCK 12  //33
#define SPI_DATA 11   //34
#define SPI_CS 18
#define SPI_DC 17
#define SPI_RESET 35

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET); //Work with fspi !
//U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, SPI_CLOCK, SPI_DATA, SPI_CS, SPI_DC, SPI_RESET); //Work !


//class ws2812b
ws2812b ws;

// Search for parameter in HTTP POST request
const char* PARAM_RGB_LED = "rgbled";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//Для UpTime
Ticker hTicker;

//Global Variables
unsigned long sUpTime;
unsigned long isec = 0; //uptime: sec
unsigned long imin = 0; //uptime: min
unsigned long ihour = 0; //uptime: hour
unsigned long iday = 0; //uptime: day


void setup() {

   Serial.begin(115200);

//PSRAM Initialisation
if(psramInit()){
        Serial.println("\nThe PSRAM is correctly initialized");
}else{
        Serial.println("\nPSRAM does not work");
}

//OLED SH1106 128x64
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Connecting Wi-Fi..");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display

  //OLED SSD1306 128X32
  disp_setup();

  //ws2812b strip
  ws.begin(GPIO_RGB_BUILTIN_LED,5);
  ws.color(0,1,2,3);
  ws.show();

 

  Serial.println("-----------------------------------------");
  Serial.printf("Total heap:\t%d \r\n", ESP.getHeapSize());
  Serial.printf("Free heap:\t%d \r\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM:\t%d \r\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM:\t%d \r\n", ESP.getFreePsram());
  Serial.printf("Flash size:\t%d (bytes)\r\n", ESP.getFlashChipSize());
  Serial.println("I2C_SDA= "+String(SDA));
  Serial.println("I2C_SCL= "+String(SCL));
  Serial.println("SPI_SCK= "+String(SCK));
  Serial.println("SPI_MOSI= "+String(MOSI));
  Serial.println("SPI_MISO= "+String(MISO));
  Serial.println("SPI_SS= "+String(SS));
  Serial.println("-----------------------------------------");

  initSPIFFS(); //инициализацич SPIFFS

    //инициализация прерывания (5 sec.)
  hTicker.attach_ms(5000, get_uptime);


//  String ssid = "tenda"; //WIFI SSID
//  String pass = "tenda_"; //WIFI PASS
  String ssid = "Alpha3"; //WIFI SSID
  String pass = "asus_"; //WIFI PASS
  //*******************************************************
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
Serial.print("IP address: ");Serial.println(WiFi.localIP());

  timeClient.begin();  //NTP Client START

  // Route for root / web page
  web_init();
  AsyncElegantOTA.begin(&server);    // Start AsyncElegantOTA
  server.begin();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());
}

uint8_t m = 0;

void loop() {

timeClient.update();
String formattedTime = timeClient.getFormattedTime();
//Serial.println(formattedTime);
String smin = formattedTime.substring(3,5);
String s2hours = formattedTime.substring(1,2);
String s1hours = formattedTime.substring(0,1);
if(s1hours == "2"){
    disp_2();
} else if(s1hours == "1"){
    disp_1();
} else {
    disp_0();
}
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn);
    u8g2.drawStr(0,63,s2hours.c_str());
    u8g2.drawStr(33,63,":");
    u8g2.drawStr(50,63,smin.c_str());
  } while ( u8g2.nextPage() );

  ws.test(m);  //LED STRIP SHOW !
 
  //Serial.println(m);
  delay(1000);
  m++;
  if ( m == 5 )
    m = 0;
    
}
