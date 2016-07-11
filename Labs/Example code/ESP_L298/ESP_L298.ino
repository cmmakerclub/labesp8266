/******************************************************************************
  Project  : NECTEC IoT Camp 2016
  Compiler : Arduino 1.6.7
  Board    : ESPresso Lite V2
  Device   : Motor Drive Module L298N, Gear Motor 1:48
  Author   : Chiang Mai Maker Club
*******************************************************************************/
#define ENA 15
#define IN1 12
#define IN2 14

#define ENB 13
#define IN3 3
#define IN4 4

void _stop();
void A_forward();
void A_backward();

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  _stop();
  delay(2000);
  A_forward();
  delay(2000);
  A_backward();
  delay(2000);
}

void _stop() {
  Serial.println("Motor Stop");
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void A_forward() {
  Serial.println("Motor A Forward");
  analogWrite(ENA, 1023);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void A_backward() {
  Serial.println("Motor A Backward");
  analogWrite(ENA, 500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

