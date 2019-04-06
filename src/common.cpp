#include <Arduino.h>
#include "common.h"
#include "user_config.h"


bool globalIsWifiConnected = false;
uint8_t globalDesiredDoorState = DOOR_STATE_ANY;
uint8_t globalDoorState = DOOR_STATE_UNKNOWN;
unsigned long globalReadStateDelay = DELAY_BETWEEN_READ_STATE_NORMAL;