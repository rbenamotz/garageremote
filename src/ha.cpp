
#include "ha.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "user_config.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "common.h"
#include <WebSocketsClient.h>
#include "common.h"
#define USE_SERIAL Serial

WebSocketsClient webSocket;

void handleSebSocketMessagee(const uint8_t *json)
{
  char outputMessage[500];
  const size_t capacity = 2 * JSON_OBJECT_SIZE(2) + 5 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + 2 * JSON_OBJECT_SIZE(6) + 970;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, json);

  const char *type = doc["type"]; // "auth_required"
  // const char *ha_version = doc["ha_version"]; // "0.91.4"
  if (strcmp("auth_required", type) == 0)
  {
    DynamicJsonDocument doc(capacity);

    doc["type"] = "auth";
    doc["access_token"] = HA_AUTH_TOKEN;
    serializeJson(doc, outputMessage, 500);
    webSocket.sendTXT(outputMessage);
    return;
  }
  if (strcmp("auth_ok", type) == 0)
  {
    DynamicJsonDocument doc(capacity);

    doc["id"] = 18;
    doc["type"] = "subscribe_events";
    doc["event_type"] = "state_changed";
    serializeJson(doc, outputMessage, 500);
    webSocket.sendTXT(outputMessage);
    return;
  }
  if (strcmp("auth_ok", type) == 0)
  {
    DynamicJsonDocument doc(capacity);

    doc["id"] = 18;
    doc["type"] = "subscribe_events";
    doc["event_type"] = "state_changed";
    serializeJson(doc, outputMessage, 500);
    webSocket.sendTXT(outputMessage);
    return;
  }
  if (strcmp("event", type) == 0)
  {
    JsonObject eventData = doc["event"]["data"];
    if (!eventData.containsKey("new_state"))
    {
      return;
    }
    eventData = eventData["new_state"];
    const char *entityId = eventData["entity_id"];
    if (strcmp(DOOR_ID, entityId) != 0)
    {
      return;
    }
    const char *newState = eventData["state"];
    bool b = (strcmp("open",newState) ==0) || (strcmp("on",newState) ==0);
    globalDoorState =  b ? DOOR_STATE_OPEN : DOOR_STATE_CLOSED;
    USE_SERIAL.printf("%s changhed to %s. Global door state is %d\n", entityId, newState, globalDoorState);
    return;
  }

  USE_SERIAL.printf("Unknow message: %s\n", json);
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    USE_SERIAL.printf("[WSc] Disconnected!\n");
    break;
  case WStype_CONNECTED:
    USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
    // webSocket.sendTXT("Connected");
    break;
  case WStype_TEXT:
    handleSebSocketMessagee(payload);
    // USE_SERIAL.printf("[WSc] get text: %s\n", payload);

    // send message to server
    // webSocket.sendTXT("message here");
    break;
  case WStype_BIN:
    USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
    hexdump(payload, length);

    // send data to server
    // webSocket.sendBIN(payload, length);
    break;
  }
}

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

void fetchDoorStateViRest()
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
  static bool b = false;
  if (!globalIsWifiConnected)
  {
    b = false;
  }
  if (!globalIsWifiConnected)
  {
    return;
  }
  if (!b)
  {
    b = true;
    fetchDoorStateViRest();
    webSocket.begin(HA_WS_HOST, HA_WS_PORT, "/api/websocket");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
    webSocket.enableHeartbeat(15000, 3000, 2);
    USE_SERIAL.printf("WebScoet client is now setup to %s:%d%s\n", HA_WS_HOST, HA_WS_PORT, "/api/websocket");
  }
  webSocket.loop();
  // static unsigned long lastStateCheck = 0lu;
  // if (!globalIsWifiConnected)
  // {
  //   return;
  // }
  // unsigned long l = millis() - lastStateCheck;
  // unsigned long delay = (globalDoorState == DOOR_STATE_UNKNOWN) ? DELAY_BETWEEN_READ_STATE_FAST : globalReadStateDelay;
  // if (l < delay)
  // {
  //   return;
  // }
  // updateDoorState();
  // lastStateCheck = millis();
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
