#include <Arduino.h>

#define SERVICE_NAME_COVER "cover"
#define SERVICE_COVER_CLOSE "close_cover"
#define SERVICE_COVER_OPEN "open_cover"

void changeCoverState(bool newState);

void setupHa();
void loopHa();