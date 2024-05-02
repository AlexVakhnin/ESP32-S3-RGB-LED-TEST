#include <Arduino.h>
#include <WiFi.h>

//Global Variables
//extern String ds1; //дисплей-строка 1
//extern String ds2; //дисплей-строка 2

//Наш список сетей
String ssid1 = "tenda"; //WIFI SSID
String pass1 = "tenda_639122"; //WIFI PASS
String ssid2 = "Alpha3"; //WIFI SSID
String pass2 = "asus_639122"; //WIFI PASS

boolean flag_ip = false;

//Определяем условный номер сети(ssid) из нашего списка сетей.
//результат 0-нет наших сетей ; 1=сеть ssid1 ; 2- сеть ssid2
int wifi_scan(){
    int res = 0; 
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n==0){Serial.println("no networks found");}
    else {
        for (int i = 0; i < n; ++i) {
            String s=WiFi.SSID(i);
            Serial.println(s);
            if (s.equals(ssid1)){res=1;break;}
            if (s.equals(ssid2)){res=2;break;}
            delay(10);
        }
        Serial.println("**********************");
    }
    WiFi.scanDelete();
    return res;
}

void wifi_init(){
  String ssid="No Netw..";
  String pass="";
  int ind = wifi_scan(); //номер сети из нашего списка
  if (ind==1){ssid=ssid1;pass=pass1;}
  else if (ind==2){ssid=ssid2;pass=pass2;}

  //ds1 = ssid; //сеть которую нашли.. на дисплей
  delay(3000);
  Serial.println("ssid="+ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address: ");Serial.println(ip);
  //ds1 = "IP.."+String(ip[2])+"."+String(ip[3]); //на дисплей

} 

void handle_wifi(){

  if (WiFi.status() != WL_CONNECTED)  {
    Serial.print(millis());
    Serial.println(" Reconnecting to WiFi...");
    //ds1="Reconnect";//на дислей
    //ds2=String(millis());
    WiFi.disconnect();
    WiFi.reconnect();
    flag_ip = true;
  } else {
    if (flag_ip){
      IPAddress ip = WiFi.localIP();
      Serial.print("IP address: ");Serial.println(ip);
      //web_reset();
      //web_init();
      //ds1 = "IP.."+String(ip[2])+"."+String(ip[3]);// на дисплей
      flag_ip = false;
    }
  }

}