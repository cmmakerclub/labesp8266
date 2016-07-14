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
#define KEY          "Dp1nis3nyzrv1sB"
#define SECRET       "lnRSV4K11OO1ypPvPzXQVANqc"
#define ALIAS        "LDR001"

#define PUBLISH_EVERY_SECS (1*1000)


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
int ldrValue = 0;
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
}

void setup() {
  _constructor();
  init_hardware();
  init_wifi();
  init_netpie();
  pinMode(A0, INPUT);
}

void loop() {
  microgear_loop();
}

void microgear_loop() {
  if (microgear.connected()) {
    microgear.loop();
    ldrValue = analogRead(A0);
    timer001.every_ms(PUBLISH_EVERY_SECS, [&]() {
      _publish();
   
    });
//    if ((millis() % 1000) == 0 ) {
//      microgear.chat("OnOff001", (char*)analogRead(A0));
//    }
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

