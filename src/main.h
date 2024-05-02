//Это внешние функции для main.cpp(слово "extern" можно опускать)

void disp_setup();
void disp_2();
void disp_1();
void disp_0();
void initSPIFFS();
void web_init();
void wifi_init();
extern void time_init();
extern void handle_time();
extern void get_uptime();