#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "BasicHttpClient.h"

const char* ssid = "benamotzim";
const char* password = "dodo4ever";

void setup () {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to ");
  Serial.println(WiFi.SSID());
  runService("switch/turn_on","switch.ge_12721_lighting_control_duplex_receptacle_switch");
  Serial.printf("going to sleep\n\n");
  ESP.deepSleep(999999999*999999999U, WAKE_NO_RFCAL);


}

void loop() {
}


void runService(String serviceName, String entityId) {

  HTTPClient http;

  // configure traged server and url
  String url = "http://192.168.1.22:8123/api/services/" + serviceName;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-ha-access", "ritb12");
  String payload = "{\"entity_id\":\"" + entityId + "\"}";
  Serial.printf("Running \"%s\" for \"%s\"\n", serviceName.c_str(), entityId.c_str());
  int httpCode = http.POST(payload);
  Serial.printf("Result: %d\n", httpCode);
  http.end();
}
