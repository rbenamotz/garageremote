#include <Arduino.h>
#include "common.h"
#include "user_config.h"

unsigned int ledFlashRate = 0;

void setupLed()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, false);
}

void handleFlash()
{
    static unsigned long lastChange = millis();
    static bool b = false;
    unsigned long l = millis() - lastChange;
    // int gg = map(l,0,ledFlashRate,0,500);
    // if (b) {
    //     gg = 500 - gg;
    // }
    // analogWrite(LED_PIN,gg);
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
        ledFlashRate = 250;
        return;
    }
    if (globalDesiredDoorState != DOOR_STATE_ANY && globalDoorState != DOOR_STATE_UNKNOWN && globalDesiredDoorState != globalDoorState)
    {
        ledFlashRate = 50;
        return;
    }
    ledFlashRate = 0;
}

void loopLed()
{
    static bool ledState = false;
    calcFlashRate();
    if (ledFlashRate > 0)
    {
        handleFlash();
        return;
    }
    bool b = (globalDoorState == DOOR_STATE_CLOSED);
    if (b == ledState)
    {
        return;
    }
    ledState = b;
    Serial.printf("Changing led state to %d\n",ledState);
    digitalWrite(LED_PIN, ledState);
}