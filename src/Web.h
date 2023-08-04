#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include "sk.h"

void writeFile(fs::FS &fs, const char * path, const char * message);
String readFile(fs::FS &fs, const char * path);
//String getSwitchState();
//double round1(double value);
//double round2(double value);
String processor(const String& var);
//String processor_serv(const String& var);
//String processor_calibr(const String& var);
//String processor_debug(const String& var);
//void abc_reset();
//void abc_set180();
//void ack_reset();
//void CO2_read();
//void abc_read();
//void ack_read();
//void status_read();
void disp_show();
//String getRSSI();
//String _modbus_hex_ack();
//String _modbus_hex_abc();
//String _modbus_hex_status();
//String _modbus_hex_CO2();
//String _modbus_hex_calibr();

//boolean modbus_command(byte mreq[], byte aout[], int count);
//unsigned int CO2count_mout();
//unsigned long ABCcount_mabc();
void rgbled_color(int nled, uint8_t red,uint8_t green,uint8_t blue);
void rgbled_show();

extern sk sk_6812;
extern const char* PARAM_RGB_LED;
extern AsyncWebServer server;
extern String ssid;
extern String pass;
extern const char* ssidPath;
extern const char* passPath;
//extern boolean flagCalibr; //признак калибровки
//extern boolean flagABCset; //установка периода ABC калибровки
//extern boolean flagABCres; //сброс периода ABC калибровки
//extern boolean flagACKres; //сброс ACK регистра
//extern float humiState;
//extern float tempState;
//extern unsigned int CO2;
//extern unsigned long CO2_round;
//extern unsigned long CO2_round1;
//extern byte status_CO2;
//extern unsigned long sFreeMem;
extern unsigned long sUpTime;
//extern unsigned long g3Time;
//extern unsigned long g2Time;
//extern unsigned long g1Time;
extern unsigned long ihour;
extern unsigned long imin;
extern unsigned long isec;
extern unsigned long iday;
//extern unsigned long ABC;
//extern byte ack_reg;
//extern int counter_1m;
//extern int counter_2m;
//extern int counter_10m;
//extern volatile unsigned int arrCO2[30];
//extern volatile unsigned int arrCO21[30];
//extern volatile unsigned int arrCO22[30];

//extern byte mout[7];
//extern byte mstatus[7];
//extern byte mabc[7];
//extern byte mack[7];
//extern byte mcalibr[8];
extern char hexChar[50];