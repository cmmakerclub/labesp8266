void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  msg[msglen] = '\0';
  Serial.printf("Incoming topic = %s, msg = %s \r\n", topic, (char *)msg);
  String msgin = String((char*)msg);

  if (msgin == "NULL") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  } else if (msgin == "RED") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(50, 0, 0));
    }
  } else if (msgin == "GREEN") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 50, 0));
    }
  } else if (msgin == "BLUE") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 50));
    }
    
  }
  pixels.show();
}
