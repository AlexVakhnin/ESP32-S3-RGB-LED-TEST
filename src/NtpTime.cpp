#include <Arduino.h>
#include "time.h"
#include "sntp.h"

void handle_time(); //декларация функции
extern String formatted_time;

unsigned long previousMillis_ntp = 0;
unsigned long interval_ntp = 10000;// 10сек

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 60*60*2; //gmt +2 = 60*60*2 -> +2 часа
const int   daylightOffset_sec = 3600; //DST = 60*60 =3600 -> +1 час



// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  //handle_time();
}



// Получаем время и заполняем переменную formatted_time
void handle_time(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_ntp >=interval_ntp) {

        struct tm timeinfo; //определяем структуру
        if(!getLocalTime(&timeinfo)){  //обращение к серверу NTP, заполнение структуры.
            Serial.println("No time available (yet)"); //ошибка при обращении к NTP
            //return;
        } else {
            //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
            char locTime[9];
            sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
            formatted_time = locTime; //'hh:mm:ss'
            //Serial.println(formatted_time);
                    }
    previousMillis_ntp = currentMillis;
    }
}


void time_init(){
    // set notification call-back function
    // By default it seems to be once an hour
    sntp_set_time_sync_notification_cb( timeavailable );

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //конфигурация клиента NTP
        // первое обращение к NTP серверу
        struct tm timeinfo; //определяем структуру
        if(!getLocalTime(&timeinfo)){  //обращение к серверу NTP, заполнение структуры.
            Serial.println("No time available (yet)"); //время не актуализировано
            //return;
        } else {
            Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
            char locTime[9];
            sprintf(locTime, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
            formatted_time = locTime; //'hh:mm:ss'
        }

}