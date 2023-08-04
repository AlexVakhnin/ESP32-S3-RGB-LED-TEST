//#include "WebProc.h"
#include "SPIFFS.h"
#include "Web.h" //OK

//Динамическая замена всех параметров на WEB странице "Sens"
String processor(const String& var){
//  Serial.println(var);
  /*if(var == "LEDSTATE"){  //это текст под графиком 
      return  getSwitchState(); //чем менять 
  }
  else */
  if (var == "RSSI"){
      return String(WiFi.RSSI());
  }
/*
  else if (var == "STATE1"){
    if (switch_flag == 0) {return "checked";}
    else { return ""; }
  }
  else if (var == "STATE2"){
    if (switch_flag == 1) {return "checked";}
    else { return ""; }
  }
  else if (var == "STATE3"){
    if (switch_flag == 2) {return "checked";}
    else { return ""; }
  }
  
  else if (var == "freemem"){
      return String(sFreeMem); 
  }
  else if (var == "humis"){
      return String(round1(humiState),1); 
  }
  else if (var == "temps"){
      return String(round1(tempState),1); 
  }
  else if (var == "co2s"){
      return String(CO2); 
  }
  */
 return String();
}
