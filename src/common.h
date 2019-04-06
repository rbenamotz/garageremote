#ifndef COMMON_H
#define COMMON_H

#define DOOR_STATE_UNKNOWN 0x00
#define DOOR_STATE_OPEN 0x01
#define DOOR_STATE_CLOSED 0x02
#define DOOR_STATE_ANY 0x03



extern bool globalIsWifiConnected;
extern uint8_t globalDesiredDoorState;
extern uint8_t globalDoorState;
extern unsigned long globalReadStateDelay;

#endif
