/******************************************************************************
* Project  : NECTEC IoT Camp 2016                                             *
* Compiler : Arduino 1.6.7                                                    *
* Board    : ESPresso Lite V2                                                 *
* Device   : PIR sensor                                                       *
* Author   : Chiang Mai Maker Club                                            *
*******************************************************************************/

#define PIR 13

void setup()
{
  // initialize Serial port
  Serial.begin(115200);

  // initialize PIR sensor
  pinMode(PIR, INPUT);

}

void loop()
{
  bool detect = digitalRead(PIR);
  if (detect == HIGH) {
    delay(200);
    Serial.println("Detect : ON");
  } else {
    Serial.println("Detect : OFF");
  }
}
