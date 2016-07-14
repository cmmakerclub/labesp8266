#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>  // v 1.1.7
#include <ESPert.h>
#include "DHT.h"

const char* ssid     = "ESPERT-3020";  // Change your ssid wifi : @ESPertAP_001 or ESPERT-002
const char* password = "espertap";  // Change your password wifi : espertap

// NETPIE.io : DEMO DHT & OLED
#define APPID   "HelloNETPIE"             // Change your appID
#define KEY     "IIHqbqzgkgy2jkQ"       // Change your Key
#define SECRET  "XQUOQIk4KBLAKCP2gUReixMId" // Change your SECRET
#define ALIAS   "DHT"       // Change your name

#define LEDPin 16
#define DHTPIN 12
#define DHTTYPE DHT22

ESPert espert;
WiFiClient client;
MicroGear microgear(client);
DHT dht(DHTPIN, DHTTYPE);

void init_hardware();
void init_wifi();

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
    digitalWrite(LEDPin, LOW);
  }
  else if (msgIN == "OFF")
  {
    digitalWrite(LEDPin, HIGH);
  }
}

void init_hardware() {
  espert.init();
  espert.oled.init();
  delay(2000);

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  dht.begin();
}

void init_wifi() {
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(PRESENT, onFoundgear);
  microgear.on(ABSENT, onLostgear);
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);

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

/************************* loop ***********************************/
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
    microgear.chat("DHT/Temperature", String(t));
    microgear.chat("DHT/Humidity", String(h));

    espert.oled.clear();
    espert.oled.setTextSize(1);
    espert.oled.setTextColor(ESPERT_WHITE);
    espert.oled.setCursor(0, 24);

    espert.oled.println("   Hello NETPIE");
    espert.oled.print("Temperature = ");
    espert.oled.println(dht.readTemperature());
    espert.oled.print("Humidity = ");
    espert.oled.println(dht.readHumidity());
    espert.oled.update();

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
