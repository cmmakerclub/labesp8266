#define MQTT_MAX_PACKET_SIZE 1024

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

const char* ssid     = "ESPERT-002";
const char* password = "espertap";


#define APPID        "HelloCMMC"
#define KEY          "jq72b0nEEd7gsxV"
#define SECRET       "b8FG3aAwOlfdXXTCiC6j1Ncpb"
#define ALIAS        "hotpotMarkI"

#define PUBLISH_EVERY_SECS (1*1000)

//define pin
#define RELAY_01_DISPENSE_PIN 3 // dispense
#define RELAY_02_REBOIL_PIN 4 // reboil

#define RELAY_03_WARM_STATUS_PIN 14 // warm status
#define RELAY_04_BOIL_STATUS_PIN 12 // boil status

#define RELAY_01_MASK 0b0000001
#define RELAY_02_MASK 0b0000010


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

#include "_publish.h"
#include "_receive.h"
#include "utils.h"

void init_wifi();
void microgear_loop();

void init_hardware() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Starting...");

  pinMode(RELAY_01_DISPENSE_PIN, OUTPUT);
  pinMode(RELAY_02_REBOIL_PIN, OUTPUT);

  digitalWrite(RELAY_01_DISPENSE_PIN, HIGH);
  digitalWrite(RELAY_02_REBOIL_PIN, HIGH);

  pinMode(RELAY_03_WARM_STATUS_PIN, INPUT);
  pinMode(RELAY_04_BOIL_STATUS_PIN, INPUT);

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
      _publish();
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

