#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

//hardware
#define BUTTON_PIN 12
#define LED_PIN 15

//Home Assistant
#define HA_END_POINT "http://<my HA URL>:8123/api/%s/%s"
#define HA_PASSW "my secret password"

#define DOOR_ID "cover.garage_door_2"


#define HOST_NAME  "garageremote2"

#define OTA_WINDOW 60000
#define DELAY_BETWEEN_READ_STATE 1000

#define WIFI_NETWORKS {"ssid1" , "passw1", "ssid2", "passw2"}
#define NUMBER_OF_WIFI_NETWORKS 2
#endif
