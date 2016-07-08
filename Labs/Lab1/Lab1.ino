#include "DHT.h"
#define BUTTON 16
#define DHTPIN 12
#define BUZZER 0
#define RELAY 15

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println(digitalRead(BUTTON));
  if (digitalRead(BUTTON) == LOW)  {  
    analogWrite(BUZZER, 1023);
    digitalWrite(RELAY, LOW);
  } else {
    analogWrite(BUZZER, 0);
    digitalWrite(RELAY, HIGH);
  }
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C \t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" % ");
  delay(200);
}
