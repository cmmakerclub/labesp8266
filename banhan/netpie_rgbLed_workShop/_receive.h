void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  msg[msglen] = '\0';
  Serial.printf("Incoming topic = %s, msg = %s \r\n", topic, (char *)msg);

  String msg2 = String((char*)msg);

  //  if (msg2 == "ON") {
  //    digitalWrite(15, !LOW);
  //    digitalWrite(LED_BUILTIN, LOW);
  //  }
  //  else if(msg2 == "OFF") {
  //    digitalWrite(15, !HIGH);
  //    digitalWrite(LED_BUILTIN, HIGH);
  //  }
  //Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  //Serial.println((char *)msg);
  Serial.print("  topic --> ");
  Serial.println((char *)topic);
  String b = "";
  Serial.print("Incoming message --> ");
  //Serial.println((char *)msg);
  int y = 0;
  for (int i = 0; i < msglen; i++) {
    char inChar = (char)msg[i];
    b += inChar;
    if (msg[i] ==  44 ) {
      if (y < 3) {
        data[y] = b.toInt();
        b = "";
      }
      y++;
//      if (y == 3) {
//        Serial.print("  last   --> ");
//        for (int x = i + 1; x < msglen; x++) {
//          char inChar = (char)msg[x];
//          b += inChar;
//          data[2] = b.toInt();
//        }
//        b = "";
//      }


    }
  }
  Serial.println(data[0]);
  Serial.println(data[1]);
  Serial.println(data[2]);
  //update rgb led
  analogWrite(redPin, data[0]);
  analogWrite(greenPin, data[1]);
  analogWrite(bluePin, data[2]);

}
