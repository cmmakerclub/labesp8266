#include <DHT.h>

#define DHTPIN 12
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
int inputPIR = 14;
int ledPin = 2;
char* state;

void read_sensor(float* t_dht, float* h_dht);
void read_PIR();


void on_prepare_data(JsonObject * root) {
  float t_dht;
  float h_dht;
  // String state;
  read_PIR();
  //read_sensor(&t_dht, &h_dht);

  JsonObject& data = (*root)["d"];
  JsonObject& info = (*root)["info"];

  data["myName"] = ALIAS;
  data["temp"] = t_dht;
  data["humid"] = h_dht;
  data["millis"] = millis();
  data["pir_state"] = state;

};


void read_sensor(float* t_dht, float* h_dht) {
  static float _t_dht;
  static float _h_dht;
  _t_dht = dht.readTemperature();
  _h_dht = dht.readHumidity();

  if (!isnan(_t_dht)) {
    *t_dht = _t_dht;
    *h_dht = _h_dht;
  }
}

void read_PIR() {
  float value = digitalRead(inputPIR);
  //static int _statet = value;
  //*state = _statet;
  char* _statet;

  if (value == HIGH)
  {
    digitalWrite(ledPin, LOW);
    //delay(1000) ;
    //*state = 9;
    _statet = "เจอบางสิ่ง";
  }

  else
  {
    digitalWrite(ledPin, HIGH);
    //*state = 0;
    _statet = "ไม่เจอ";
  }
  
  state = _statet;

  //microgear.publish("stste",(char*)x);
}

