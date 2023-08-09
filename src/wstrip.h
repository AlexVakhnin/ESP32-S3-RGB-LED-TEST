/*
КАК С ЭТИМ РАБОТАТЬ :)

#include "wstrip.h"

#define GPIO_RGB_BUILTIN_LED 21 //GPIO21 для уравления

wstrip ws; //class ws create

ws.begin( GPIO_RGB_BUILTIN_LED, 1);  //инициализация на 1 светодиод в ленте

ws.color(0,1,2,3); //параметры: номер_светодиода_в_ленте=0,  Red=1, Green=2, Blue=3
ws.show(); //записать данные в ленту 

*/


#ifndef WSTRIP_H
#define WSTRIP_H 1


#include "Arduino.h"

class wstrip {

public:

wstrip(void){} //конструктор
~wstrip(void){  //деструктор
   
   //free( _led_data); //освобождаем память массива шаблонов
    delete[] _led_data;
    _led_data = NULL;
    delete[] _color;
    _color = NULL;
}


//инициализация обьекта
//параметры: номер PIO, количество светодиодов в ленте
void begin(int gpio, int nled) {

//приватные свойства..
_gpio=gpio;
_nled=nled;
_nbit=24*nled;

//_led_data = (rmt_data_t *)calloc(  _nbit, sizeof( rmt_data_t ) ); //выделяем память
_led_data = new rmt_data_t[_nbit];
_color = new int[3];

//вызываем конструктор с параметрами (ном.GPIO, режим_передачи=true, mem_block_num=1)
if (( _rmt_send = rmtInit( _gpio, RMT_TX_MODE, RMT_MEM_64)) == NULL)
{
        Serial.println("RMT init sender failed\n");  //если обьект не был создан..
}

float realTick = rmtSetTick(_rmt_send, 100);//определяет аремя одного такта передатчика
Serial.printf("RMT real tick set to: %fns\n", realTick);

}

//установка цвета одного светодиода в ленте
//параметры: ном_светодиода_с_нуля, R, G, B
void color(int led, uint8_t red,uint8_t green,uint8_t blue)
{
//color[0] = 0x55; color[01] = 0x11, color[02] = 0x77 ; // RGB value
_color[0] = red; _color[01] = green, _color[02] = blue ; // RGB value

int led_index = led; //номер светодиода в ленте

// если не входит в диаазон (0, _nled-1), ставим на первый в ленте
if ((led_index)>=_nled) {
    led_index = 0;
}
// переменные для текущих значений
int col; //текущий номер цветового канала (0, 2)
int bit; //текущий бит цветового канала (0, 7)

int i=led_index*24; //вычисляем начало отсчета в массиве шаблонов

    for (col=0; col<3; col++ ) {   //проход по цветовым каналам RGB -3 канала
        for (bit=0; bit<8; bit++){  //проход о всем битам цв.канала - 8 бит

        if (_color[col] & (1<<(7-bit)))  {
            _led_data[i].level0 = 1;
            _led_data[i].duration0 = 8;
            _led_data[i].level1 = 0;
            _led_data[i].duration1 = 4;
        } else {
            _led_data[i].level0 = 1;
            _led_data[i].duration0 = 4;
            _led_data[i].level1 = 0;
            _led_data[i].duration1 = 8;
        }
        i++;
        }
    }

}


// послать управляющую последовательность светодиодной ленте
void show()
{
// Send the data
rmtWrite(_rmt_send, _led_data, _nbit);
delay(100);
}


private:

rmt_data_t* _led_data = NULL; //массив шаблонов RMT
rmt_obj_t* _rmt_send = NULL;  //ссылка на обьект(класс) rmt_obj_t
int _gpio; //номер GPIO
int _nled; //количество светодиодов в ленте
int _nbit; //количество бит управления для всей ленты 
//int _color[] = { 0x0, 0x0, 0x0 }; // RGB value
int* _color = NULL; //массив каналов цвета RGB

};
#endif