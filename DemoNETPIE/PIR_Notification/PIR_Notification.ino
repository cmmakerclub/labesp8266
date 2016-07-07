#include <Arduino.h>
#include <ESPert.h>
#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <ESP8266HTTPClient.h>

#ifndef WIFI_SSID
  #define WIFI_SSID       "ESPERT-002"
  #define WIFI_PASSPHRASE "espertap"
#endif

String smartphone_key = "5003057247027200";
String message = "Warning!!!%20PIR%20Detect"; // %20 is mean Spacebar (" ")

ESPert espert;
WiFiConnector wifi(WIFI_SSID, WIFI_PASSPHRASE);

#define PIR 13
#define LED 16

void init_hardware();
void init_wifi();
void doHttpGet();

void setup() {
  init_hardware();
  init_wifi();
  wifi.connect();
}

void loop() {
  wifi.loop();
  if (wifi.connected()) {
    bool detect = digitalRead(PIR);
    if (detect == HIGH) {
      delay(200);
      Serial.println("Detect : Off");
      digitalWrite(LED, LOW);
      doHttpGet();
    } else {
      Serial.println("Detect : Off");
      digitalWrite(LED, HIGH);
    }
    delay(500);
  }
}

void init_hardware()  {
  espert.init();
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
}

void init_wifi() {
  wifi.init();
  wifi.on_connected([&](const void* message)
  {
    Serial.print("WIFI CONNECTED WITH IP: ");
    Serial.println(WiFi.localIP());
  });

  wifi.on_connecting([&](const void* message)
  {
    Serial.print("Connecting to ");
    Serial.println(wifi.get("ssid") + ", " + wifi.get("password"));
    delay(200);
  });
}

void doHttpGet() {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  String msg = "http://www.espert.io/MySmartphone/send?key=" + smartphone_key + "&message=" + message;
  http.begin(msg); //HTTP
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
      Serial.print(payload);
      Serial.println(" OK");
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}
