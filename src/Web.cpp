//#include <Arduino.h>
#include "ArduinoJson.h"
//#include <ESPAsyncWebServer.h>
//#include "SPIFFS.h"
#include "Web.h"  //внешние функции

//обработка текста переданного клиентом заросом htttp post
void onConnectBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  StaticJsonDocument<200> doc; //резервируем место в памяти

  auto error = deserializeJson(doc, data);  //разбор json структуры
  if (error) {
    Serial.print(F("deserializeJson() failed with code "));  //ошибка разметки
    Serial.println(error.c_str());
    return;
  }
  JsonObject root = doc.as<JsonObject>(); //у нас не массив , а обьект json

  for (JsonPair keyValue : root) { //проход о всем парам обьекта json

    if (keyValue.key() == PARAM_RGB_LED){   //словили знакомый параметр json
      String val = keyValue.value().as<const char*>();  //получили значение параметра
      //обработка значения параметра json
      Serial.println("POST: PARAM_RGBLED ->"+val);
      if (val =="red") {
        //RED
        //sk_6812.color(0,0,60,0,0); //GRB
        //sk_6812.show();
        //rgbled_color(0, 60, 0, 0);
        //rgbled_show();
        ws.color(0,60,0,0);
        ws.show();
        Serial.println("LED_BUILTIN = RED");   
      } else if (val =="blue"){
        //BLUE
        //sk_6812.color(0,0,0,60,0); //GRB
        //sk_6812.show();
        //rgbled_color(0, 0, 0, 60);
        //rgbled_show();
        ws.color(0,0,0,60);
        ws.show();
        Serial.println("LED_BUILTIN = BLUE");   
      } else if (val =="green"){
        //GREEN
        //sk_6812.color(0,60,0,0,0); //GRB
        //sk_6812.show();
        //rgbled_color(0, 0, 60, 0);
        //rgbled_show();
        ws.color(0,0,60,0);
        ws.show();
        Serial.println("LED_BUILTIN = GREEN");
      } else {
        Serial.println("Error: Value Unknown");
      }
    }
  }

}
 


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void web_init(){

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  
//POST
//---------------------------------------------------------------------------

server.on("/posts", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain","POSTED SUCCESSFULLY");
}, NULL, onConnectBody);

//---------------------------------------------------------------------------
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  
  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send(200, "text/html", "<h1>SOFT RESET..</h1>");
    delay(3000);
    ESP.restart();
  });

  //Это ответ сервера на AJAX запрос из WEB страницы"
  server.on("/jsonstate", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncResponseStream *response = request->beginResponseStream("application/json");    
    const int capacity = JSON_OBJECT_SIZE(6);//Количество живых параметров JSON
    StaticJsonDocument<capacity> doc;
    doc["rssi"] = WiFi.RSSI();
    //doc["humis"] = round2(humiState);
    //doc["temps"] = round2(tempState);
    //doc["co2"] = CO2;
    doc["freemem"] = ESP.getFreeHeap();  //или String().c_str();???????
    doc["iday"] = iday;
    doc["ihour"] = ihour;
    doc["imin"] = imin;
    doc["isec"] = isec;
    //doc["g3diff"] = sUpTime - g3Time;
    //doc["g2diff"] = sUpTime - g2Time;
    serializeJson(doc, *response);
    request->send(response);
  });



server.onNotFound(notFound);
  
}
