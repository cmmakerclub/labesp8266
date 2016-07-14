/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : 5V Relay Module
  Author   : Chiang Mai Maker Club
*******************************************************************************/
#define RELAY 14

void setup() {
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);
}

void loop() {
  digitalWrite(RELAY, HIGH);
  delay(1000);
  digitalWrite(RELAY, LOW);
  delay(1000);
}
