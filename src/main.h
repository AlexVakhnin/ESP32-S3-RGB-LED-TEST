//Это внешние функции для main.cpp(слово "extern" можно опускать)

//void getGraphArr(); //вызывается раз в 5 сек.
void get_uptime();//вызывается раз в 5 сек.
//void serial2_clear();
void disp_setup();
//void ip_show(String mod, IPAddress ip);
//void wifi_show(int i);
String readFile(fs::FS &fs, const char * path);
void initSPIFFS();
void web_init();
//void handler_2s_job();
//void handler_1s_job();
void rgbled_begin();
void rgbled_color(int nled, uint8_t red,uint8_t green,uint8_t blue);
void rgbled_show();