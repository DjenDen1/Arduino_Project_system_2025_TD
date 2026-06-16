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
/*
#include <DHT.h>
#include <SPI.h>
#include <U8glib.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <LM75A.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define CS_PIN  22 
#define BMP_CS 53

DHT dht(DHTPIN, DHTTYPE);

// --- настройки OLED ---
U8GLIB_SSD1306_128X64 u8g(0x3C);
Adafruit_BMP280 bmp(BMP_CS);
LM75A sensorlm;
// --- переменные ---
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

  // --- ПК → HC-05 ---
  if (Serial.available()) 
  {
      
      Serial.println("");Serial.println("");
      Serial.print("NONSTOP_PRINTING (COM PORT)");
      Serial.println("");Serial.println("");

      Serial.print("Temp: "); Serial.println(temperature);
      Serial.print("Hum: ");  Serial.println(humidity);
      Serial.print("Times: ");Serial.println(times);

      //Serial1.println("----com port was used by mega2560---");

      Serial.print("Температура(bmp): ");
      Serial.print(temp);
      Serial.println(" *C");

      Serial.print("Давление(bmp280): ");
      Serial.print(pressure);
      Serial.println(" hPa");

      Serial.print("температура(LM75A): ");
      Serial.println(lmtemp);

      Serial.print("Температура(DHT22): ");
      Serial.println(temperature);
  }

 
  if (Serial1.available()) 
  {
      
      Serial1.println("");Serial1.println("");
      Serial1.print("NONSTOP_PRINTING (BLUETOOTH PORT)");
      Serial1.println("");Serial1.println("");

      Serial1.print("Temp: "); Serial1.println(temperature);
      Serial1.print("Hum: ");  Serial1.println(humidity);
      Serial1.print("Times: ");Serial1.println(times);

//      Serial.println("----bluetooth port was used by mega2560----");

      Serial1.print("Температура(bmp): ");
      Serial1.print(temp);
      Serial1.println(" *C");

      Serial1.print("Давление(bmp280): ");
      Serial1.print(pressure);
      Serial1.println(" hPa");

      Serial1.print("температура(LM75A): ");
      Serial1.println(lmtemp);

      Serial1.print("Температура(DHT22): ");
      Serial1.println(temperature);

     
     Serial1.read();
  }

  
  drawOLED();

  delay(1200);    
}


*/