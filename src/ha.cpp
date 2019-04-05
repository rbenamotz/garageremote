
#include "ha.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "user_config.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

void preHttpClient(HTTPClient *http, const char *url)
{
  char authHeader[200];
  sprintf(authHeader, "Bearer %s", HA_AUTH_TOKEN);
  Serial.print("Trying to connect to ");
  Serial.println(url);
  // WiFiClient client;
  http->begin(url);
  http->addHeader("Content-Type", "application/json");
  http->addHeader("Authorization", authHeader);
}

int readState(String entityId)
{
  HTTPClient http;
  char url[200];
  sprintf(url, "%s/api/states/%s", HA_END_POINT, DOOR_ID);
  preHttpClient(&http, url);
  int httpCode = http.GET();
  if (httpCode < 200 || httpCode >= 300)
  {
    Serial.printf("Could not read state for %s. http code: %d\n", entityId.c_str(), httpCode);
    http.end();
    return -1;
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  StaticJsonDocument<bufferSize> doc;

  DeserializationError error = deserializeJson(doc, http.getString());

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return 0;
  }

  // JsonObject &root = jsonBuffer.parseObject(http.getString());
  String temp = doc["state"];
  http.end();
  // Serial.printf("Read state for %s: %s\n",entityId.c_str(),temp.c_str());
  if (temp == "on" || temp == "open")
  {
    return 1;
  }
  return 0;
}

void changeCoverState(bool newState)
{
  //POST http://home.benamotz.com:8123/api/services/cover/close_cover
  HTTPClient http;
  char url[200];
  if (newState)
  {
    sprintf(url, "%s/api/services/cover/open_cover", HA_END_POINT);
  }
  else
  {
    sprintf(url, "%s/api/services/cover/close_cover", HA_END_POINT);
  }
  preHttpClient(&http, url);
  char payLoad[200];
  sprintf(payLoad, "{\"entity_id\" : \"%s\"} ", DOOR_ID);
  Serial.println(payLoad);
  int httpCode = http.POST(payLoad);
  http.POST(payLoad);
  Serial.printf("Result: %d\n", httpCode);
  http.end();
}
