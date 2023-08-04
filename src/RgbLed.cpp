#include "Arduino.h"

//порт управления GPIO 21
#define BUILTIN_RGBLED_PIN 21

//количество диодов в ленте - всего 32 RGB диода
#define NR_OF_LEDS 1//8*4 

//количество бит данных для всей ленты
//для уравления одним диодом нужно выдать 24 бита данных (8 бит на один цвет)
#define NR_OF_ALL_BITS 24*NR_OF_LEDS

// Времена полупериодов передачи одного бита для WS2812B RGB диода
//
// передаем - "0"
// уровни:
// |высокий - 0.4us |низкий - 0.85 0us |
//
// передаем - "1"
// уровни:
// |высокий - 0.8us |низкий - 0.4us |


//обьявления
rmt_data_t led_data[NR_OF_ALL_BITS]; //массив шаблонов по количеству всех бит
rmt_obj_t* rmt_send = NULL;  //ссылка на обьект(класс) rmt_obj_t

int color[] = { 0x0, 0x0, 0x0 }; // RGB value
//int led_index ; //текущий светодиод в ленте


void rgbled_begin()
{
//вызываем конструктор с параметрами (ном.GPIO, режим_передачи=true, mem_block_num=1)
if ((rmt_send = rmtInit(BUILTIN_RGBLED_PIN, RMT_TX_MODE, RMT_MEM_64)) == NULL)
{
        Serial.println("init sender failed\n");  //если обьект не был создан..
}

float realTick = rmtSetTick(rmt_send, 100);//определяет аремя одного такта передатчика
    Serial.printf("real tick set to: %fns\n", realTick);
}


void rgbled_color(int nled, uint8_t red,uint8_t green,uint8_t blue)
{
//color[0] = 0x55; color[01] = 0x11, color[02] = 0x77 ; // RGB value
color[0] = red; color[01] = green, color[02] = blue ; // RGB value
int led_index = nled; //номер светодиода в ленте


// если не входит в диаазон, ставим на первый в ленте
if ((led_index)>=NR_OF_LEDS) {
    led_index = 0;
}
// Init data with only one led ON
int /*led,*/ col, bit;
//int i=0;
int i=led_index*24; //вычисляем начало отсчета в массиве шаблонов

//for (led=0; led<NR_OF_LEDS; led++) {  //проход по всем светодиодам в ленте - NR_OF_LEDS
    for (col=0; col<3; col++ ) {   //проход по цветовым каналам RGB -3
        for (bit=0; bit<8; bit++){  //проход о всем битам цв.канала - 8 бит

        if ( (color[col] & (1<<(7-bit))) /*&& (led == led_index)*/ ) {
            led_data[i].level0 = 1;
            led_data[i].duration0 = 8;
            led_data[i].level1 = 0;
            led_data[i].duration1 = 4;
        } else {
            led_data[i].level0 = 1;
            led_data[i].duration0 = 4;
            led_data[i].level1 = 0;
            led_data[i].duration1 = 8;
        }
        i++;
        }
    }
//}

}

void rgbled_show()
{
// Send the data
rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
delay(100);
}