/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : DS18B20
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float temp;
void setup()
{
  Serial.begin(115200);
  sensors.begin();
}

void loop()
{ 
  sensors.requestTemperatures(); 
  temp = sensors.getTempCByIndex(0);
  
  Serial.print("Temperature is ");  
  Serial.println(temp); 
  delay(1000); 
}
