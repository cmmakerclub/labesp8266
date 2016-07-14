void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  Serial.print(topic);
  Serial.print(" : ");
  char strState[msglen];
  for (int i = 0; i < msglen; i++)
  {
    strState[i] = (char)msg[i];
    Serial.print((char)msg[i]);
  }

  Serial.println();

  String stateStr = String(strState);

  Serial.print("Topic is ");
  Serial.println(topic);

  //###############


  //################

  //  if (stateStr[0] == 'W')
  //  {
  //    Serial.println("GOT W");
  //    digitalWrite(LEDPin, LOW);
  //  }
  //  if (stateStr[0] == 'S')
  //  {
  //    Serial.println("GOT S");
  //    digitalWrite(LEDPin, HIGH);
  //  }
  //
  //  Serial.printf("NAT: %d %c \r\n", stateStr[0], stateStr[0]);

  if (stateStr[0] ==  'B') { // Stop
    Serial.println("Stop");
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
    analogWrite(IN1, 0);
    analogWrite(IN2, 1023);
    analogWrite(IN3, 0);
    analogWrite(IN4, 1023);
   // delay(1000);
  }
  if (stateStr[0] == 'W') {  // forward
    Serial.println("Forward");
    analogWrite(ENA, 500);
    analogWrite(ENB, 0);
    analogWrite(IN1, 1023);
    analogWrite(IN2, 0);
    //delay(1000);
  }
  if (stateStr[0] == 'S') { // backward
    Serial.println("Backward");
    analogWrite(ENA, 500);
    analogWrite(ENB, 0);
    analogWrite(IN1, 0);
    analogWrite(IN2, 1023);
    //delay(1000);
  }
  if (stateStr[0] == 'A') { // turn left
    Serial.println("Turn left");
    analogWrite(ENB, 1023);
    analogWrite(IN3, 1023);
    analogWrite(IN4, 0);
    //delay(1000);
  }
  if (stateStr[0] == 'D') { // turn right
    Serial.println("Turn right");
    analogWrite(ENB, 1023);
    analogWrite(IN3, 0);
    analogWrite(IN4, 1023);
    //delay(1000);
  }
}
