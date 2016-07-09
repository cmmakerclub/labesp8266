//  EXAMPLE CODE FOR NECTEC IOT CAMP 2016 by Chiang Mai Maker Club
//  Device : ESPresso lite v2, Soil moisture, RELAY module
//  Dashboard : Soil_dashboard


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MicroGear.h>  // v 1.1.7

const char* ssid     = "ssid wifi";  // Change your ssid wifi
const char* password = "password wifi";  // Change your password wifi

// NETPIE.io : HelloNETPIE : NETPIE_01
#define APPID   "HelloNETPIE"             // Change your appID
#define KEY     "xxxxxxxxxxx"             // Change your Key
#define SECRET  "xxxxxxxxxxx"             // Change your SECRET
#define ALIAS   "SoilFreeboard"           // Change your name

#define SOIL 13
#define RELAY 14

boolean soilState = false;

WiFiClient client;
MicroGear microgear(client);

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

    // อ่านค่าจากเซ็นเซอร์ Soil moisture sensor
    soilState = digitalRead(SOIL);
    
    if (soilState == true) {
      // ส่งค่าไปยัง netpie.io
      microgear.chat("SoilFreeboard/moisture", "DRY");
    } else {
      microgear.chat("SoilFreeboard/moisture", "WET");
    }
    
    Serial.print("Soil moisture = ");
    Serial.println(soilState);
    delay(200);
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
  digitalWrite(RELAY, HIGH);
}

