#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Ticker.h>
//#include <SPI.h>
#include <U8g2lib.h>
//#include "sk.h"
#include "ws2812b.h"

//-----------------
#include "main.h"   //определяет функции во внешних файлах
//-----------------


//Управдение внутренним RGB светодиодом
#define GPIO_RGB_BUILTIN_LED 21

//SPI for SH1106 OLED 128x64
#define SPI_CLOCK 12  //33
#define SPI_DATA 11   //34
#define SPI_CS 18
#define SPI_DC 17
#define SPI_RESET 35

//SPIClass * fspi = NULL;

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, SPI_CS, SPI_DC, SPI_RESET); //Work with fspi !
//U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, SPI_CLOCK, SPI_DATA, SPI_CS, SPI_DC, SPI_RESET); //Work !


//sk sk_6812; //class sk create !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

//fspi = new SPIClass(FSPI);
//alternatively route through GPIO pins of your choice
//Reassign SPI Custom GPIO !!!
//fspi->begin(SPI_CLOCK, -1/*36*/, SPI_DATA/*, SPI_CS*/); //SCLK, MISO, MOSI, SS

//OLED SH1106 128x64
  u8g2.begin();
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  u8g2.drawStr(0,10,"Connectin Wi-Fi..");	// write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display

  //OLED SSD1306 128X32
  disp_setup();


  //GRB LED
  //sk_6812.begin( GPIO_RGB_BUILTIN_LED, 1);  //init RGB LED class 
  //sk_6812.color(0,2,1,3,0); //GRBW
  //sk_6812.show();

  //rgbled_begin();
  //rgbled_color(0, 2, 1, 3);
  //rgbled_show();

  ws.begin(GPIO_RGB_BUILTIN_LED,1);
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
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
Serial.print("IP address: ");Serial.println(WiFi.localIP());

  // Route for root / web page
  web_init();
  server.begin();

  delay(100);
  Serial.printf("Free heap after create objects:\t%d \r\n", ESP.getFreeHeap());



}

uint8_t m = 24;

void loop() {

  char m_str[3];
  strcpy(m_str, u8x8_u8toa(m, 2));		/* convert m to a string with two digits */
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_logisoso62_tn);
    u8g2.drawStr(0,63,"2");
    u8g2.drawStr(33,63,":");
    u8g2.drawStr(50,63,m_str);
  } while ( u8g2.nextPage() );
  delay(1000);
  m++;
  if ( m == 60 )
    m = 0;
    
}
