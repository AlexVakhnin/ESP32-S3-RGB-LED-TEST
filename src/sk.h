/*
КАК С ЭТИМ РАБОТАТЬ :)

#include "sk.h"

#define GPIO_RGB_BUILTIN_LED 21 //GPIO для уравления

sk sk_6812; //class sk create

sk_6812.begin( GPIO_RGB_BUILTIN_LED, 1);  //инициализация на 1 светодиод в ленте
sk_6812.color(0,2,1,3,4); //номер светодиода в ленте=0, Green=2, Red=1, Blue=3, White=4
sk_6812.show(); //записать данные в ленту 

*/


#ifndef SK_H
#define SK_H 1


#include "driver/rmt.h"


//Use RMT for a strip of SK6812 RGBW leds



class sk{
public:
sk(void){} //конструктор
~sk(void){  //деструктор
   
   free( _skstrip);
   rmt_driver_uninstall( skconfig.channel );
}


/*---------------------------------------Инициализация RMT----------------------------------*/
//параметры: (номер GPIO), (кол. светодиодов в ленте)
int begin( int pin, int count){

//глобальные переменные (свойства объекта)
_bitcount = count*32; //количество бит на выдачу
_ledpin = pin;  //номер GPIO esp32 
_ledcount = count; //количество светодиодов в ленте
_brightness = 100; //яркость глобальный процент

int rc;
    //занимаем память из кучи под массив переменных типа rmt_item32_t
    // тип rmt_item32_t - это шаблон для передачи 1-го бита (на 1 LED-> 32 шаблона нужно)
    // _bitcount - полное количество бит передаваемое в ленту LED
    // calloc функция при этом, все это заполняет нулями.. 

    _skstrip = (rmt_item32_t *)calloc(  _bitcount, sizeof( rmt_item32_t ) );

    //Заполняем структуру типа rmt_config_t определенную в драйвере -> в локальное свойство объекта
     skconfig.rmt_mode = RMT_MODE_TX; //мы передаем..
     skconfig.channel = RMT_CHANNEL_1;  //канал RMT = 1
     skconfig.gpio_num = (gpio_num_t)_ledpin; //мапируем GPIO для работы RMT
     skconfig.mem_block_num = 1;
     skconfig.tx_config.loop_en = 0;
     skconfig.tx_config.carrier_en = 0; //несущая отключена т.к не ИК пульт
     skconfig.tx_config.idle_output_en = 1;
     skconfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
     skconfig.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH; //несущая -> лог. 1
     //делитель тактовой - определяет время одного такта
     skconfig.clk_div = 8; // 80MHx / 8 = 10MHz translates to  0,1 us = 100 ns per count
 
     rmt_config( &skconfig); //конфигурируем RMT
     rc = rmt_driver_install( skconfig.channel, 0, 0);  //  rmt_driver_install(rmt_channel_t channel, size_t rx_buf_size, int rmt_intr_num)    
 
 return(rc);
}

/*-------------------------------------------------------------------------*/
//собираем 4 байта в одну 32-битную переменную
uint32_t getcolor( uint8_t r,uint8_t g,uint8_t b,uint8_t w ){

uint32_t  kleur=0;

kleur |= ((uint32_t)g<<24);
kleur |= ((uint32_t)r<<16);
kleur |= ((uint32_t)b<<8);
kleur |= (uint32_t)w; 

return( kleur );
}

/*-------------------------------------------------------------------------*/
//из 32-х битного целого выделяем 4 байта (по ссылкам..)
uint32_t breakcolor( uint32_t kleur, uint8_t *r,uint8_t *g,uint8_t *b,uint8_t *w ){

*g = (kleur >> 24 )&0xff;
*r = (kleur >> 16 )&0xff;
*b = (kleur >> 8 )&0xff;
*w = kleur&0xff;

return( kleur );
}

/*-------------------------------------------------------------------------*/
//Устанавливаем цвет с помощью одной 32-битной переменной
void color32( int led, uint32_t kleur, int brightness = -1  ){
uint8_t r,g,b,w;

breakcolor( kleur,&r,&g,&b,&w); //заполняем rgbw из uint32_t kleur
color( led, r,g,b,w, brightness);
  
}

/*-------------------------------------------------------------------------*/
//установка цвета для конкретного LEDа с помощью значений RGBW и яркости (основная функция)
void color( int led, uint8_t red,uint8_t green,uint8_t blue,uint8_t white, int brightness = -1  ){
uint32_t  kleur=0, bright;
uint8_t   r = red, g = green, b = blue, w = white; 
int       i,bit;

if ( led < 0 || led >= ledcount() ) return; //номер св.диода в цепочке - проверка..

//Serial.printf( "---\nrgbw = %d.%d.%d.%d\n", red,green,blue,white);
// brightness is a percentage 0...100

bright = brightness;  //яркость в процентах по умолчанию - 100%
if ( brightness == -1 ) bright = 100; 

bright = (bright * _brightness) /100;
    
r = bright*red/100;
g = bright*green/100;
b = bright*blue/100;
w = bright*white/100;

//Serial.printf("_brightness = %d, brightness %d, bright %d\n", _brightness, brightness, bright);
//Serial.printf( "rgbw = %d.%d.%d.%d\n---\n", r,g,b,w);

kleur |= ((uint32_t)g<<24);  //заполняем kleur как GRBW  4-байта
kleur |= ((uint32_t)r<<16);
kleur |= ((uint32_t)b<<8);
kleur |= (uint32_t)w; 

//Serial.printf("Set color of led %d kleur(RGBW) %08X\n", led, kleur); //RGBW реально ?????????

// sk6812 has around 600us/600us 1, 300/900us 0
// WS2812 has around 800us/500us 1, 400/900us 0

//заполняем участок массива шаблонов  _skstrip для конкретного светодиода
//всего 32 цикла, i= смещ.-смещ+32 (смещение = 0,32,64,..для каждого LED)
//тайминги исравлены для WS2812B !!!

for (i=(led*32),bit=0; bit<32; bit++){  //проход о всем битам kleur (32 бита начиная со старшего)
     
     if ( (kleur & (1<<(31-bit)))  ) {    //передаем лог-1                                            
           _skstrip[i].level0 = 1;
           _skstrip[i].duration0 = 8;  //время высокого уровня сигнала (6-SC6812)
           _skstrip[i].level1 = 0;
           _skstrip[i].duration1 = 5;  //время низкого уровня сигнала (6-SC6812)

      } else {                            //передаем лог-0
           _skstrip[i].level0 = 1;
           _skstrip[i].duration0 = 4;  //время высокого уровня сигнала (3-SC6812)
           _skstrip[i].level1 = 0;
           _skstrip[i].duration1 = 9;  //время низкого уровня сигнала (9-SC6812)
      }
      //if ( bit == 31 )  _skstrip[i].duration1 += 60;
      ++i;    
}  

return;
}


/*-------------------------------------------------------------------------*/
//физически выдать последовательность на ленту светодиодов из RMT
void show(){

    int rc;
    // esp_err_t rmt_write_items(rmt_channel_t channel, rmt_item32_t *rmt_item, int item_num, bool wait_tx_done)

    //заставляет RMT выдать последовательность бит основываясь на массиве шаблонов
    // параметры: (номер канала RMT), (массив шаблонов), (количество всех бит=шаблонов), (1-ожидаем завершения)
    rc = rmt_write_items(skconfig.channel, _skstrip, _bitcount, 1);
    
    
    delay(2);

}

/*-------------------------------------------------------------------------*/
//погасить всю ленту светодиодов
void clear( void ){

for ( int i =0; i < _ledcount;++i ) {  //проход о всей ленте
  color(i,0,0,0,0);
}
show();
delay(10);
}
/*-------------------------------------------------------------------------*/
//Установка свойства - яркость
uint8_t   setbrightness(uint8_t newbrightness ){
  _brightness = newbrightness;
  return( _brightness );
}
/*-------------------------------------------------------------------------*/
//получение свойства - яркость
uint8_t   getbrightness(){
  return( _brightness );
}
/*-------------------------------------------------------------------------*/
//получение свойства - колич. светодиодов в ленте
int ledcount(){
return( _ledcount);
}

/*-------------------------------------------------------------------------*/
//объявление внутренних переменных - свойств объекта
private:
int   _ledcount; //колич. светодиодов в ленте
int   _bitcount;
int   _ledpin;
int   _brightness;  //текущая яркость
rmt_item32_t* _skstrip = NULL; //ссылка на массив для всего стрима
rmt_config_t skconfig; //структура для конфигурации RMT
float _realtick;

};

#endif
