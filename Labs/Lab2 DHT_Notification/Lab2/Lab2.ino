//  EXAMPLE CODE FOR NECTEC IOT CAMP 2016 by Chiang Mai Maker Club
//  Device : ESPresso lite v2, DHT11, RELAY module
//  Dashboard : DHT_dashboard
//  Library : DHT-sensor-library

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>  // v 1.1.7
#include <ESP8266HTTPClient.h>  // v 1.1.1
#include "DHT.h"

const char* ssid     = "ssid wifi";  // Change your ssid wifi
const char* password = "password wifi";  // Change your password wifi

String smartphone_key = "smartphone key";   //  Change your smartphone key
String message = "Hello%20from%20ESPressoLITE"; //  Change your message

#define DHTPIN 12
#define DHTTYPE DHT11

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void init_wifi();
void init_hardware();
void doHttpGet(String);

void setup() {
  init_wifi();
  init_hardware();
  Serial.println("Init done...");
}

void loop() {
  // อ่านค่าจากเซ็นเซอร์ DHt22
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (WiFi.status() == WL_CONNECTED) {
    if (t >= 30) {  //  if Temperature > 30 c
      Serial.println("Sent message....");
      String msg = "http://www.espert.io/MySmartphone/send?key=" + smartphone_key + "&message=" + message;
      doHttpGet(msg);
      delay(5000);  //  delay for gethttp
    }
    Serial.println("Connect...");
  } else  {
    Serial.println("connection lost, reconnect...");
    WiFi.begin(ssid, password);
    delay(500);
  }

  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("\t");
  Serial.print("Humidity = ");
  Serial.println(h);
  delay(200);
}
/******************* initial loop ***********************************/
void init_wifi() {
  Serial.begin(115200);
  delay(200);

  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void init_hardware() {
  dht.begin();
}

void doHttpGet(String msg) {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");

  http.begin(msg); // GET HTTP
  Serial.println(msg);

  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    Serial.print("[CONTENT]\n");

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
