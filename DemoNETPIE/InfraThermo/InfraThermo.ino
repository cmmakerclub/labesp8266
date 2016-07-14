#define MQTT_MAX_PACKET_SIZE 1024

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "timer.hpp""
#include <Adafruit_MLX90614.h>
#include "CMMC_Blink.hpp"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
CMMC_Blink blinker;

const char* ssid     = "ESPERT - 3020";
const char* password = "espertap";

// NETPIE.io : DEMO Device (CupCoffee)
#define APPID        "HelloNETPIE"
#define KEY          "IQXoe2GAAL3vYgk"
#define SECRET       "g9g9yK8LQYTbqHMgbaNLapdEX"
#define ALIAS        "InfraTemp"

#define LED 2

bool mlx_initialised = false;

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

void init_hardware();
void init_wifi();
void microgear_loop();

void setup() {
  _constructor();
  init_hardware();
  init_wifi();
  init_netpie();
}

void loop() {
  microgear_loop();
}

void init_hardware() {
  blinker.init();
  Serial.begin(115200);
  Serial.println("Starting...");
  blinker.blink(50, LED_BUILTIN);
  delay(200);

  if (!mlx_initialised) {
    mlx_initialised = true;
    mlx.begin();
  }

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
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
  blinker.blink(200, LED_BUILTIN);
  //microgear.resetToken();
}

