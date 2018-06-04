
#include "ha.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "user_config.h"


int readState(String entityId) {
  char url[200];
  HTTPClient http;
  sprintf(url,HA_END_POINT ,"states",entityId.c_str());
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-ha-access", HA_PASSW);
  int httpCode = http.GET();
  if (httpCode < 200 || httpCode >= 300) {
    Serial.printf("Could not read state for %s. http code: %d\n",entityId.c_str(),httpCode);
    http.end();
    return -1;
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(http.getString());
  String temp = root["state"];
  http.end();
  // Serial.printf("Read state for %s: %s\n",entityId.c_str(),temp.c_str());
  if (temp == "on" || temp == "open") {
    return 1;
  }
  return 0;
}


float readVolume(String entityId) {
  char url[200];
  HTTPClient http;
  sprintf(url,HA_END_POINT,"states",entityId.c_str());
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-ha-access", HA_PASSW);
  int httpCode = http.GET();
  if (httpCode < 200 || httpCode >= 300) {
    http.end();
    return 0.0;
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(http.getString());
  float temp = root["attributes"]["volume_level"];
  http.end();
  return temp;
}

void runService(String domain, String operation, String entityId) {
  char url[200];
  HTTPClient http;
  String temp = domain + "/" + operation;
  sprintf(url,HA_END_POINT,"services",temp.c_str());
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-ha-access", HA_PASSW);
  String payload = "{\"entity_id\":\"" + entityId + "\"}";
  Serial.printf("Running \"%s.%s\" for \"%s\"\n", domain.c_str(),operation.c_str(), entityId.c_str());
  int httpCode = http.POST(payload);
  http.POST(payload);
  Serial.printf("Result: %d\n", httpCode);
  http.end();
}
