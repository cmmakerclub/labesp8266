 void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.printf("Incoming topic = %s, msg = %s \r\n", topic, msg);
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  String msg2 = String((char*)msg);

  if (msg2 == "ON") {
    digitalWrite(15, !LOW);
    digitalWrite(LED_BUILTIN, LOW);    
  }
  else if(msg2 == "OFF") {
    digitalWrite(15, !HIGH);
    digitalWrite(LED_BUILTIN, HIGH);        
  }
  else {
    
  }
  
}
