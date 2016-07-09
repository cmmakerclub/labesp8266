//  EXAMPLE CODE FOR NECTEC IOT CAMP 2016 by Chiang Mai Maker Club
//  Device : ESPresso lite v2, DHT11, RELAY module
//  Dashboard : DHT_dashboard
//  Library : DHT-sensor-library

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>  // v 1.1.7
#include "DHT.h"

const char* ssid     = "ESPERT-002";  // Change your ssid wifi : @ESPertAP_001 or ESPERT-002
const char* password = "espertap";  // Change your password wifi : espertap

// NETPIE.io : man_DHT
#define APPID   "HelloCMMC"             // Change your appID
#define KEY     "3CGFOspur1y1hVF"       // Change your Key
#define SECRET  "T1eUhstPrXG86hyFtNTuFmpD0" // Change your SECRET
#define ALIAS   "DHTFreeboard"       // Change your name

#define DHTPIN 12
#define DHTTYPE DHT11
#define RELAY 15

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
    microgear.chat("DHTFreeboard/Temperature", String(t));
    microgear.chat("DHTFreeboard/Humidity", String(h));

    Serial.print("Temperature = ");
    Serial.print(t);
    Serial.print("\t");
    Serial.print("Humidity = ");
    Serial.println(h);
    delay(500);
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
    delay(10);
  }
  else if (msgIN == "OFF")
  {
    digitalWrite(RELAY, HIGH);
    delay(10);
  }
}

/******************* initial loop ***********************************/
void init_wifi() {
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(PRESENT, onFoundgear);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);

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

  Serial.println("connecting netpie.io...");
  //microgear.resetToken();
  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
  Serial.println("netpie.io connected.");
}

void init_hardware() {
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  dht.begin();
}

