#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

//hardware
#define BUTTON_PIN 2
#define LED_PIN 4

//Home Assistant
//For example: http://myhome.com:8123
#define HA_END_POINT "http://myhome.com:8123"
//Create long live token from account profile page in HA
#define HA_AUTH_TOKEN "<token goes here>"

#define DOOR_ID "cover.garage_door"

#define HOST_NAME "garageremote"

#define OTA_WINDOW 60000
#define DELAY_BETWEEN_READ_STATE_NORMAL 20000
#define DELAY_BETWEEN_READ_STATE_FAST 1000

//Add wifi credentials for home WiFi and phone access point
#define WIFI_NETWORKS                        \
    {                                        \
        "ssid1", "passw2", "ssid2", "passw2" \
    }
#define NUMBER_OF_WIFI_NETWORKS 2
#endif
