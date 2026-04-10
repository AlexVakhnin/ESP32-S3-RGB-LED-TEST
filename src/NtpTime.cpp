#include <Arduino.h>
#include "time.h"
//#include "sntp.h"
#include "esp_sntp.h"

void handle_time(); //декларация функции
extern String formatted_time;

unsigned long previousMillis_ntp = 0;
unsigned long interval_ntp = 30000;// 30сек время обновления табло..

const char* ntpServer = "pool.ntp.org"; //это основной
//const char* ntpServer = "ua.pool.ntp.org"; //должен сам определять, что UA
//const long  gmtOffset_sec = 60*60*2; //gmt +2 = 60*60*2 -> +2 часа зона
const long  gmtOffset_sec = 60*60*3; //gmt +3 = 60*60*2 -> +3 часа зона
//const int   daylightOffset_sec = 3600; //DST = 60*60 =3600 -> +1 час корректировка
const int   daylightOffset_sec = 0; // 0 часов корректировка (работает не корректно..)



// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  //handle_time();
}



// Получаем время и заполняем переменную formatted_time (T=30 sec)
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
    // set notification call-back function - обращение к NTP
    // By default it seems to be once an hour - по умолчанию раз в час
    sntp_set_time_sync_notification_cb( timeavailable );

    //-------
    // Set interval NTP in milliseconds (e.g., 5 min)
    uint32_t interval = 5 * 60 * 1000; 
    sntp_set_sync_interval(interval);
    //-------

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