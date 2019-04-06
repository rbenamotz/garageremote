
#include "ha.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "user_config.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "common.h"

void prepHttpClient(HTTPClient *http, const char *url)
{
  char authHeader[200];
  sprintf(authHeader, "Bearer %s", HA_AUTH_TOKEN);
  // Serial.print("Trying to connect to ");
  // Serial.println(url);
  static WiFiClient client;
  http->begin(client, url);
  http->addHeader("Content-Type", "application/json");
  http->addHeader("Authorization", authHeader);
}

void updateDoorState()
{
  HTTPClient http;
  char url[200];
  sprintf(url, "%s/api/states/%s", HA_END_POINT, DOOR_ID);
  prepHttpClient(&http, url);
  int httpCode = http.GET();
  if (httpCode < 200 || httpCode >= 300)
  {
    Serial.printf("Could not read state for %s. http code: %d\n", DOOR_ID, httpCode);
    http.end();
    globalDoorState = DOOR_STATE_UNKNOWN;
  }
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
  StaticJsonDocument<bufferSize> doc;
  DeserializationError error = deserializeJson(doc, http.getString());
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    Serial.println(http.getString());
    globalDoorState = DOOR_STATE_UNKNOWN;
  }

  String temp = doc["state"];
  http.end();
  globalDoorState = (temp == "on" || temp == "open") ? DOOR_STATE_OPEN : DOOR_STATE_CLOSED;
  Serial.printf("%d: Updated door state. New state is %d\n", millis(), globalDoorState);
}

void setupHa()
{
}
void loopHa()
{
  static unsigned long lastStateCheck = 0lu;
  if (!globalIsWifiConnected)
  {
    return;
  }
  unsigned long l = millis() - lastStateCheck;
  unsigned long delay = (globalDoorState == DOOR_STATE_UNKNOWN) ? DELAY_BETWEEN_READ_STATE_FAST : globalReadStateDelay;
  if (l < delay)
  {
    return;
  }
  updateDoorState();
  lastStateCheck = millis();
}

void changeCoverState(bool newState)
{
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
  prepHttpClient(&http, url);
  char payLoad[200];
  sprintf(payLoad, "{\"entity_id\" : \"%s\"} ", DOOR_ID);
  Serial.println(url);
  Serial.println(payLoad);
  int httpCode = http.POST(payLoad);
  http.POST(payLoad);
  Serial.printf("Result: %d\n", httpCode);
  http.end();
}
