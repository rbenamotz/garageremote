#include <Arduino.h>

#define SERVICE_NAME_COVER "cover"
#define SERVICE_COVER_CLOSE "close_cover"
#define SERVICE_COVER_OPEN "open_cover"

int readState(String entityId);
float readVolume(String entityId);
void runService(String domain, String operation, String entityId);
