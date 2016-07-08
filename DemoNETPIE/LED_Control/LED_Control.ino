#define MQTT_MAX_PACKET_SIZE 1024

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "timer.hpp""

const char* ssid     = "ESPERT-002";
const char* password = "espertap";

// NETPIE.io : DEMO Device (LED Control)
#define APPID        "HelloCMMC"
#define KEY          "b5BZBHHKguYe1lV"
#define SECRET       "jdXPKpLifhbvrEdBZqBHJoY1L"
#define ALIAS        "LEDcontrol"

#define LEDPIN 16
#define PUBLISH_EVERY_SECS (2*1000)

WiFiClient client;
CMMC_Interval timer001;

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
  Serial.println("Starting...");
  pinMode(LEDPIN, OUTPUT);
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
  microgear.resetToken();
}

