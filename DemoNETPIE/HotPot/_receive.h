void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  msg[msglen] = '\0';
  Serial.printf("Incoming topic = %s, msg = %s \r\n", topic, (char *)msg);

  String msg2 = String((char*)msg);

  if (msg2 == "h") {
    digitalWrite(RELAY_02_REBOIL_PIN, LOW);
    delay(2000);
    digitalWrite(RELAY_02_REBOIL_PIN, HIGH);
  }
  else if (msg2 == "d") {
    digitalWrite(RELAY_01_DISPENSE_PIN, LOW);
    delay(1000);
    digitalWrite(RELAY_01_DISPENSE_PIN, HIGH);
  }

}
