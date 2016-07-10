/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : Potentiometer
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#define POT A0

byte potValue;

void setup() {
  Serial.begin(115200);
}

void loop() {
  potValue = analogRead(POT);
  
  Serial.print("POT = ");
  Serial.println(potValue);
  delay(200);
}
