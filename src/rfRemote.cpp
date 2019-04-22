#include <Arduino.h>
#include "user_config.h"
#include "common.h"

unsigned long remoteOnTime = 0;

void setupRfRemote()
{
    pinMode(RF_REMOTE_PIN, OUTPUT);
    digitalWrite(RF_REMOTE_PIN, false);
}
void loopRfRemote()
{
    unsigned long l = millis() - remoteOnTime;
    if (l > 1000)
    {
        digitalWrite(RF_REMOTE_PIN, false);
    }
}

void clickRfRemote()
{
    Serial.println("Toggling with RF");
    digitalWrite(RF_REMOTE_PIN, true);
    remoteOnTime = millis();
}