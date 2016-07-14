/*  NETPIE ESP8266 basic sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include "ESPert_OLED.hpp"
const char* ssid     = "ESPERT-3020";
const char* password = "espertap";

#define APPID        "HelloNETPIE"
#define KEY          "ab0dNrc15TRwLtD"
#define SECRET       "l7UMUJgdcamCvXHuJGvrZXWyJ"
#define ALIAS        "MK_Remote"

WiFiClient client;
AuthClient *authclient;

int timer = 0;
MicroGear microgear(client);
//set On_off status
bool on_off = 0;
//set dim
unsigned int dim = 0;

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}

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

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  /* Set the alias of this microgear ALIAS */
  microgear.setName(ALIAS);
}
CMMC_OLED oled;
void setup() {
  /* Add Event listeners */

  oled.init();
  oled.clear();
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.println("12345678901234567890");
  oled.update();
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, onFoundgear);

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, onLostgear);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

}

void loop() {
  /* To check if the microgear is still connected */
  if (microgear.connected()) {
    //    Serial.println("connected");

    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();

    if (digitalRead(13) == 1) {
      oled.clear();
      oled.setCursor(64, 25);
      oled.setTextSize(10);
      Serial.println(on_off);
      if (on_off == 1)
      {
        oled.print("255");
        microgear.chat("plugDIM001", "255");
        on_off = 0;
      }
      else
      {
        oled.print("0");
        microgear.chat("plugDIM001", "0");
        on_off = 1;
      }
      oled.update();
    }
    if (digitalRead(12) == 1) {
      oled.clear();
      if (dim <= 255)
      {
        dim += 5;
      }
      if (dim > 255)
      {
        dim = 255;
      }
      oled.setCursor(64, 25);
      oled.setTextSize(10);
      oled.print(dim);
      oled.update();
      microgear.chat("plugDIM001", (String)dim);
    }
    if (digitalRead(14) == 1) {
      oled.clear();
      if (dim >= 0)
      {
        dim -= 5;
      }
      if (dim < 0)
      {
        dim = 0;
      }
      oled.setCursor(64, 25);
      oled.setTextSize(10);
      oled.print(dim);
      oled.update();
      microgear.chat("plugDIM001", (String)dim);
    }
    delay(200);
  }
  else {
    Serial.println("connection lost, reconnect...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}
