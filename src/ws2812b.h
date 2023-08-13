#ifndef WS2812B_H
#define WS2812B_H 1


#include "Arduino.h"

class ws2812b {

public:

ws2812b(void){} //конструктор
~ws2812b(void){  //деструктор
   
   //free( _led_data); //освобождаем память массива шаблонов
    delete[] _led_data;
    _led_data = NULL;
    delete[] _color;
    _color = NULL;
    delete[] _led_color;
    _led_color = NULL;
}


//инициализация обьекта
//параметры: номер PIO, количество светодиодов в ленте
void begin(int gpio, int nled) {

//приватные свойства..
_gpio=gpio;
_nled=nled; //количество светодиодов в ленте
_nbit=24*nled;

//_led_data = (rmt_data_t *)calloc(  _nbit, sizeof( rmt_data_t ) ); //выделяем память
_led_data = new rmt_data_t[_nbit];

_color = new int[3];
_led_color = new uint32_t[nled];
for (int i=0;i< nled;i++) _led_color[i] = 0;

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
_color[0] = green; _color[01] = red, _color[02] = blue ; // RGB value

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

        if (_color[col] & (1<<(7-bit)))  { //анализ одного бита нач.со старшего
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

//_led_color[led] = getcolor(red, green, blue); //заполняем массив цветов для этого LED

}


// послать управляющую последовательность светодиодной ленте
void show()
{
// Send the data
rmtWrite(_rmt_send, _led_data, _nbit);
delay(100);
}

//погасить всю ленту светодиодов
void clear( void ){

for ( int i =0; i < _nled;++i ) {  //проход о всей ленте
  color(i,0,0,0);
}
//show();
//delay(10);
}

//отобразить массив цветов на массив шаблонов ленты
void refresh( void ){

for ( int i =0; i < _nled;++i ) {  //проход о всей ленте

  color24(i, _led_color[i]);
}

}

//продвинуть ленту на один шаг
//параметр - цвет первого светодиода ленты (24бит)
void push( uint32_t color ){

for (int i=_nled-1;i>0;i--)
    {
    //Serial.print(String(i)+",");
    _led_color[i] = _led_color[i-1];
    }
  _led_color[0] = color;

    refresh();
}


//собираем 3 байта в одну 24-битную переменную
uint32_t getcolor( uint8_t r,uint8_t g,uint8_t b){

uint32_t  kleur=0;

kleur |= ((uint32_t)r<<16);
kleur |= ((uint32_t)g<<8);
kleur |= (uint32_t)b;

return( kleur );
}

//из 24-х битного целого выделяем 3 байта (по ссылкам..)
uint32_t breakcolor( uint32_t kleur, uint8_t *r,uint8_t *g,uint8_t *b ){

*r = (kleur >> 16 )&0xff;
*g = (kleur >> 8 )&0xff;
*b = kleur&0xff;

return( kleur );
}

//Устанавливаем цвет с помощью одной 32-битной переменной
void color24( int led, uint32_t kleur){
uint8_t r,g,b;

breakcolor( kleur,&r,&g,&b); //заполняем rgbw из uint32_t kleur
color( led, r,g,b);
  
}


void test(int tn){
    int x = rand()%30;
    clear();
    if(tn==0){
        //color(tn,60,0,0);
        push(getcolor(60,x,0));
    }else if(tn==1){
        //color(tn,0,60,0);
        push(getcolor(0,60,x));
    }else if (tn==2){
        //color(tn,0,0,60);
        push(getcolor(x,0,60));
    }else if(tn==3){
        //color(tn,20,20,20);
        push(getcolor(0,0,20));
    }else{
        //color(tn,0,20,20);
        push(getcolor(0,20,0));
    }
    show();
}

private:
uint32_t* _led_color = NULL; //массив цветов всех светодиодов в ленте
rmt_data_t* _led_data = NULL; //массив шаблонов RMT
rmt_obj_t* _rmt_send = NULL;  //ссылка на обьект(класс) rmt_obj_t
int _gpio; //номер GPIO
int _nled; //количество светодиодов в ленте
int _nbit; //количество бит управления для всей ленты 
//int _color[] = { 0x0, 0x0, 0x0 }; // RGB value
int* _color = NULL; //массив каналов цвета RGB

};
#endif