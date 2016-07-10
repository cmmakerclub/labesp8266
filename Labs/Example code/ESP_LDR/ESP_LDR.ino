/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : LDR
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#define LDR A0

void setup()
{
  // initialize Serial port
  Serial.begin(115200);
}

void loop()
{
  // read the input on analog pin 0
  int sensorValue = analogRead(LDR);

  // print out the value you read:
  Serial.println(sensorValue);
  delay(100);
}
