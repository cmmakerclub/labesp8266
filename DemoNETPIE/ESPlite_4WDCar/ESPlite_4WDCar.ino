#include <AuthClient.h>
#include <MicroGear.h>
#include <ArduinoJson.h>
#include <MQTTClient.h>
#include <SHA1.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <MicroGear.h>
#include "timer.hpp"

#define MQTT_MAX_PACKET_SIZE 1024

const char* ssid     = "ESPERT-3020";
const char* password = "espertap";


#define APPID        "HelloNETPIE"
#define KEY          "RsqYS6C2QkT7FUw"
#define SECRET       "PIspQh38KXg0z1nyTL4OSJgLv"
#define ALIAS        "car"

#define LEDPin 16
#define DHTPIN 12
#define DHTTYPE DHT22

#define PUBLISH_EVERY_SECS (2*1000)

WiFiClient client;
AuthClient *authclient;
CMMC_Interval timer001;
CMMC_Interval timer002;

// MQTT CONNECTOR CONCEPT
char jsonStrbuffer[1024];
JsonObject *cmmc_root;
JsonObject *d;
JsonObject *cmmc_info;

StaticJsonBuffer<800> jsonRootBuffer;
StaticJsonBuffer<512> jsonDBuffer;

int timer = 0;
MicroGear microgear(client);

#define ENA 1
#define ENB 3
#define IN1 12
#define IN2 13
#define IN3 14
#define IN4 15

#include "_publish.h"
#include "_receive.h"
#include "utils.h"

void init_wifi();
void microgear_loop();

void init_hardware() {
  Serial.begin(115200);
  pinMode(LEDPin, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println("Starting...");
}

void setup() {
  _constructor();
  init_hardware();
  init_wifi();
  init_netpie();
}

void loop() {
  microgear_loop();
}

void microgear_loop() {
  if (microgear.connected()) {
    microgear.loop();
    timer001.every_ms(PUBLISH_EVERY_SECS, [&]() {
//      _publish();
    });
  }
  else {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
  }
}

void init_wifi() {
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

