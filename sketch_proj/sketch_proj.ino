#include <DHT.h>
#include <SPI.h>
#include <U8glib.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LM75A.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define CS_PIN   22 
#define BMP_CS 53

DHT dht(DHTPIN, DHTTYPE);

U8GLIB_SSD1306_128X64 u8g(0x3C);
Adafruit_BMP280 bmp(BMP_CS);
LM75A sensorlm;

float temperature = 0;
float humidity = 0;
int times = 0;
char cmd = ' ';
float temp = 0;
float pressure = 0;
float lmtemp = 0;

void drawOLED() {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_6x13);

    u8g.setPrintPos(0, 13);
    u8g.print("Temp: ");
    u8g.print(lmtemp, 1);
    u8g.print(" C");

    u8g.setPrintPos(0, 26);
    u8g.print("Hum: ");
    u8g.print(humidity, 1);
    u8g.print(" %");

    u8g.setPrintPos(0, 40);
    u8g.print("Pressure: ");
    u8g.print(pressure, 1);

    u8g.setPrintPos(0, 54);
    u8g.print("Times: ");
    u8g.print(times);
    u8g.print(" and cmd ");
    u8g.print(cmd);

  } while (u8g.nextPage());
}

void setup() {
  Serial.begin(9600);   
  Serial1.begin(9600); 
  bmp.begin(0x76);
  dht.begin();
  SPI.begin();
  Wire.begin();

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
}

void loop() 
{
  temp = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0F;
  lmtemp = sensorlm.getTemperatureInDegrees();
  
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Ошибка DHT!");
    temperature = 0;
    humidity = 0;
  }

  if (Serial1.available()) 
  {
      cmd = Serial1.read(); 
      times++; 
 
      Serial1.print(lmtemp, 1);      
      Serial1.print(";");
      Serial1.print(humidity, 1);    
      Serial1.print(";");
      Serial1.print(pressure, 1);    
      Serial1.println("");           
  }

  drawOLED();
  delay(100); 
}
