void on_prepare_data(JsonObject * root) {
  JsonObject& data = (*root)["d"];
  JsonObject& info = (*root)["info"];

  data["myName"] = ALIAS;
  data["millis"] = millis();
};

