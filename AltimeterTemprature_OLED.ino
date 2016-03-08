/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!;

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_BMP085.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

unsigned int counter = 0;
unsigned int minutes = 0;
unsigned int hours = 0;

int inPin1 = 3;
int inPin2 = 4;
int state1 = 0;
int state2 = 0;
int led = 13;
int scroll = 0;
float averageTemp = 0;

Adafruit_BMP085 bmp;


void setup()   {               
  
  Serial.begin(9600);
  pinMode(inPin1, INPUT_PULLUP);
  pinMode(inPin2, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  
  
  
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  if (!bmp.begin()) 
  {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
  display.display();
  delay(2000);

  }


void loop() {
  
  if (bmp.begin())
  {
    
    state1 = !digitalRead(3);
    state2 = !digitalRead(4);
    
    if(state1) // display Altitude
    {    
      display.clearDisplay();
      display.setTextSize(4);
      display.setTextColor(WHITE);
      display.setCursor(24,20);
      //display.print(bmp.readAltitude(101124),1);
      
      for(int i=0; i<30; i++) 
      {        
        averageTemp = averageTemp + bmp.readAltitude(101124);
        delay(5); 
      }      

      averageTemp = averageTemp/30;   
  
      //averageTemp = averageTemp + bmp.readAltitude(101124);
      //averageTemp = averageTemp /2;
        
      display.print((averageTemp),1);
      display.setTextSize(1);
      display.setCursor(24,50);
      display.print("  Altitude(m) ");     
      display.display();
    } 
    else if(state2) //display Temprature
    {    
      
      display.clearDisplay();  
     
      display.drawRect(scroll, 0, 5, 5, WHITE);
      display.drawRect(scroll, 1, 4, 4, WHITE);
      display.drawRect(scroll, 2, 3, 3, WHITE);
      display.drawRect(scroll, 3, 2, 2, WHITE);      
      display.setTextSize(4);
      display.setCursor(20,20);
      display.setTextColor(WHITE);
      display.print(bmp.readTemperature(),1);
      display.setTextSize(1);
      display.print("   Temperature(C)");  
      display.drawRect(13, 17, 105, 34, WHITE);
      display.display(); 
      scroll = scroll + 5;
      
      if(scroll>125) { scroll = 0; }
    }
    else //display Preasure
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("Pressure: ");
      display.print(bmp.readPressure());
      display.println(" Pa"); 

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print("Temperature: ");
      display.print(bmp.readTemperature(),1);
      display.println(" C");  

      display.setTextSize(1);
      display.setTextColor(WHITE);
      //display.setCursor(0,0);
      display.print("Altitude: ");
      display.print(bmp.readAltitude(101124));
      display.println(" M");   
      display.println("");
      display.print("H:");
      display.print(hours);
      display.print(" M:");
      display.print(minutes);
      display.print(" S:");
      display.print(counter); 
      display.display();     
    }
    
   if(counter<120)
   {
     counter++;   
     if(counter==120 && minutes<60 )
     {
       minutes++;
       counter = 0;  
       if(minutes==60)
       {
         hours++;
         minutes=0;
       }
     }
   }
   
   
   
  }
  else
  {
    display.clearDisplay();
    display.println("Sensor Error");   
    display.display();
   
  }
  
  digitalWrite(led, HIGH);   
  delay(200);
  digitalWrite(led, LOW);    
  
}
