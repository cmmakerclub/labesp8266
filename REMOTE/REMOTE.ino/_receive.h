 void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  msg[msglen] = '\0';  
  Serial.printf("Incoming topic = %s, msg = %s \r\n", topic, (char *)msg);
  
  String msg2 = String((char*)msg);
  Serial.print("Incoming Message >>>> ");
  Serial.println(msg2);
//  if (msg2 == "ON") {
//    digitalWrite(15, !LOW);
//    digitalWrite(LED_BUILTIN, LOW);    
//  }
//  else if(msg2 == "OFF") {
//    digitalWrite(15, !HIGH);
//    digitalWrite(LED_BUILTIN, HIGH);        
//  }
  
}
