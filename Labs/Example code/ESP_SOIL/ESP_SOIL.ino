/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : Soil moisture
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#define SOIL A0
int soilValue;

void setup()
{
  // initialize Serial port
  Serial.begin(115200);
}

void loop()
{
  //read the input on analog pin 0
  soilValue = analogRead(SOIL);

  // print out the value you read:
  Serial.println(soilValue);
  delay(200);
}
