#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ha.h"
#include "GarageRemote.h"
#include <ESP8266mDNS.h>
#include "oth.h"
#include "cover.h"
#include "user_config.h"
#include "button.h"




ESP8266WiFiMulti wifiMulti;
volatile bool desired_state = false;
bool waiting_for_state_change = false;
bool is_door_open = false;
unsigned long last_read_state = 0;
String wifi_networks[] = WIFI_NETWORKS;



void reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }
  Serial.print("Connecting");
  while (wifiMulti.run() != WL_CONNECTED) {
    indicateWaitingForWiFi();
    Serial.print(".");
  }
  Serial.print("\nConnected to ");
  Serial.println(WiFi.SSID());
  if (MDNS.begin(HOST_NAME)) {
    Serial.printf("MDNS started. Host name: %s\n",HOST_NAME);
  }
}

void setup () {
  Serial.begin(115200);
  for (int i=0; i< NUMBER_OF_WIFI_NETWORKS * 2; i+=2) {
    wifiMulti.addAP(wifi_networks[i].c_str(),wifi_networks[i+1].c_str());
  }
  reconnect();
  setupOTA();
  setupButton();
  pinMode(LED_PIN,OUTPUT);
  desired_state = readState (DOOR_ID);
}




void changeDoorState(bool isOpen) {
  if (isOpen != is_door_open) {
    Serial.printf("state changed from %d to %d\n",is_door_open,isOpen);
  }
  is_door_open = isOpen;
  digitalWrite(LED_PIN,!is_door_open);
}



void loop() {
  reconnect();
  if (!need_to_run_service) {
    loopOTA();
  }
  if (need_to_run_service) {
    indicateRunningService();
    waiting_for_state_change = true;
    need_to_run_service = false;
    Serial.println("Running servce");

    if (is_door_open) {
      runService(SERVICE_NAME_COVER,SERVICE_COVER_CLOSE,DOOR_ID);
    } else {
      runService(SERVICE_NAME_COVER,SERVICE_COVER_OPEN,DOOR_ID);
    }
    desired_state = !desired_state;
  }
  if (millis() - last_read_state < DELAY_BETWEEN_READ_STATE) {
    delay(10);
    return;
  }
  int temp = readState (DOOR_ID);
  last_read_state = millis();
  if (temp == -1) {
    indicateError();
    return;
  }
  bool newState = (temp == 1);
  bool isNewStateSameAsDesired = (newState==desired_state);
  changeDoorState(newState);

  if (!waiting_for_state_change) {
    return;
  }
  if (isNewStateSameAsDesired) {
    changeDoorState(newState);
    waiting_for_state_change = false;
    return;
  }
  indicateWaitingForStateToChange();
}
