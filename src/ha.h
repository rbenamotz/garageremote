#include <Arduino.h>

#define SERVICE_NAME_COVER "cover"
#define SERVICE_COVER_CLOSE "close_cover"
#define SERVICE_COVER_OPEN "open_cover"

int readState(String entityId);
void changeCoverState(bool newState);