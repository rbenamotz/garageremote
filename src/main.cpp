#include <Arduino.h>
#include "wifi.h"
#include "button.h"
#include "led.h"
#include "ha.h"
#include "rfRemote.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n-----------------------------------------\nHello from Smart Garage button");
  setupWifi();
  setupHa();
  setupLed();
  setupButton();
  setupRfRemote();
}
void loop()
{
  loopWifi();
  loopButton();
  loopHa();
  loopLed();
  loopRfRemote();
}
