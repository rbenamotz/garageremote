#include <Arduino.h>
#include "common.h"
#include "user_config.h"

unsigned int ledFlashRate = 0;

void setupLed()
{
    pinMode(LED_PIN, OUTPUT);
}

void handleFlash()
{
    static unsigned long lastChange = 0lu;
    static bool b = true;
    unsigned long l = millis() - lastChange;
    if (l < ledFlashRate)
    {
        return;
    }
    digitalWrite(LED_PIN, b);
    b = !b;
    lastChange = millis();
}
void calcFlashRate()
{
    if (!globalIsWifiConnected)
    {
        ledFlashRate = 500;
        return;
    }
    if (globalDesiredDoorState != DOOR_STATE_ANY && globalDoorState != DOOR_STATE_UNKNOWN && globalDesiredDoorState != globalDoorState)
    {
        ledFlashRate = 100;
        return;
    }
    ledFlashRate = 0;
}

void loopLed()
{
    calcFlashRate();
    if (ledFlashRate > 0)
    {
        handleFlash();
        return;
    }
    bool b = (globalDoorState == DOOR_STATE_OPEN);
    digitalWrite(LED_PIN, b);
}