#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define I2C_CLOCK 15
#define I2C_DATA 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// print "1"
void disp_1(){
  display.clearDisplay();
  //display.drawRect(0,0,32,128,WHITE); //full region
  int y0= 43;
  display.fillRect(32-9,y0,9,85,WHITE);//43+85=128; 32-11+11=32; !!! 

  display.fillRect(32-11*2+1+2,y0+11+1,11,11,WHITE);
  display.fillTriangle(32-11*2+1+2+2, y0+11+1, 32-11+2, y0+11+1, 32-11+2, y0+2+1, WHITE);//X1,Y1,X2,Y2,X3,Y3
  display.fillTriangle(32-11*2+1+2+2, y0+11+1+11-1, 32-11+2-1, y0+11+1+11-1, 32-11+2-1,  y0+11+1+11-4-1-1, BLACK);//X1,Y1,X2,Y2,X3,Y3
  display.display();//show
}




void disp_setup(){
  //ReDefine I2C Pins
  Wire.begin(I2C_DATA, I2C_CLOCK);
  // Setup display SSD1306
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.setRotation(3);
  display.display(); //show
  delay(1000);
  display.setTextColor(WHITE);
  display.clearDisplay();
  //display.setCursor(0, 0);
  //display.setTextSize(8);  
  //display.print("1");

  display.drawRect(0,43,32,84,WHITE);
  display.drawRect(0,0,32,32,WHITE);
  display.display();//show
  delay(1000);
  disp_1();

}