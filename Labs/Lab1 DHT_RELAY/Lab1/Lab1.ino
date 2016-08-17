/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : DHT11, RELAY module
  Dashboard : DHT_dashboard
  Library : DHT-sensor-library, CMMC_Blink
  Author   : Chiang Mai Maker Club
*******************************************************************************/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>  // v 1.1.7
#include "DHT.h"
#include "CMMC_Blink.hpp"
CMMC_Blink blinker;

const char* ssid     = "ssid wifi";  // Change your ssid wifi 
const char* password = "password wifi";  // Change your password wifi

// NETPIE.io : lab_device
#define APPID   "HelloNETPIE"             // Change your appID
#define KEY     "VrMaS5XpxrMmwVH"       // Change your Key
#define SECRET  "OdzDsJP8pB7SvBYtbUzjuAZ6G" // Change your SECRET
#define ALIAS   "lab1"              // Change your name

#define DHTPIN 12
#define DHTTYPE DHT11
#define RELAY 14

WiFiClient client;
MicroGear microgear(client);
DHT dht(DHTPIN, DHTTYPE);

void init_wifi();
void init_hardware();


void setup() {
  init_wifi();
  init_hardware();
  Serial.println("Init done...");
}

void loop() {
  if (microgear.connected())
  {
    microgear.loop();

    // อ่านค่าจากเซ็นเซอร์ DHt22
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    
    // ส่งค่าไปยัง netpie
    microgear.chat("lab1/Temperature", String(t));
    microgear.chat("lab1/Humidity", String(h));

    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print("\t");
    Serial.print("Humidity = ");
    Serial.println(h);
    delay(1000);
  }
  else
  {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
  }
}

/******************* microgear loop ***********************************/
void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setName("mygear");
}

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  Serial.print(topic);
  Serial.print(" : ");

  char Str_msg[msglen];
  for (int i = 0; i < msglen; i++)
  {
    Str_msg[i] = (char)msg[i];
    Serial.print((char)msg[i]);
  }

  Serial.println();

  String msgIN = String(Str_msg).substring(0, msglen);

  Serial.print("Topic is ");
  Serial.println(topic);

  if (msgIN == "ON")
  {
    digitalWrite(RELAY, LOW);
    delay(100);
  }
  else if (msgIN == "OFF")
  {
    digitalWrite(RELAY, HIGH);
    delay(100);
  }
}

/******************* initial loop ***********************************/
void init_wifi() {
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(PRESENT, onFoundgear);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);

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

  Serial.println("connecting netpie.io...");
  //microgear.resetToken();
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
  blinker.detach();
  Serial.println("netpie.io connected.");
}

void init_hardware() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  dht.begin();
}

