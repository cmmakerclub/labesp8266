/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : PIR
  Dashboard : -
  Library : CMMC_Blink
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>  // v 1.1.1
#include "DHT.h"
#include "CMMC_Blink.hpp"
CMMC_Blink blinker;

const char* ssid     = "ESPERT-3020";  // Change your ssid wifi
const char* password = "espertap";  // Change your password wifi

String smartphone_key = "5003057247027200";   //  Change your
String message = "Motion%20Detected"; //  Change your message

WiFiClient client;

const int sleepTimeS = 9999; // 300 = 30นาที

void init_wifi();
void init_hardware();
void doHttpGet(String);

void setup() {
  init_wifi();
  init_hardware();
  Serial.println("Init done...");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Sent message....");
    String msg = "http://www.espert.io/MySmartphone/send?key=" + smartphone_key + "&message=" + message;
    doHttpGet(msg);
    delay(5000);  //  delay for gethttp
    ESP.deepSleep(sleepTimeS * 6000000);
    Serial.println("Connect...");
  } else  {
    Serial.println("connection lost, reconnect...");
    WiFi.begin(ssid, password);
    delay(500);
  }
}

/******************* initial loop ***********************************/
void init_wifi() {
  blinker.init();
  blinker.blink(50, LED_BUILTIN);
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
  blinker.blink(200, LED_BUILTIN);
}

void init_hardware() {
  blinker.detach();
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
