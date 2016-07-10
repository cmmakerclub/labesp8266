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
#define KEY          "gTjbJoeD771QMUB"
#define SECRET       "doCguDPHfQL3bhMpemRamzVYg"
#define ALIAS        "banhan_rgbLed"

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
// data keep value rgb
int data[3] = {0,0,0};
//led pin  r=12 g=13 g = 0
int redPin = 4;
int greenPin = 13;
int bluePin = 0;

MicroGear microgear(client);

#include "_publish.h"
#include "_receive.h"
#include "utils.h"

void init_wifi();
void microgear_loop();

void init_hardware() {
  Serial.begin(115200);
  //set pin output for drive led
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(15, OUTPUT);
  //
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.println("Starting...");
}

void setup() {
  _constructor();
  init_hardware();
  init_wifi();
  init_netpie();
  //test drive led
  analogWrite(redPin,data[0]);
  analogWrite(greenPin,data[1]);
  analogWrite(bluePin,data[2]);
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


