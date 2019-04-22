#include <Arduino.h>
#include "button.h"
#include "user_config.h"
#include "common.h"
#include "ha.h"
#include "rfRemote.h"

// volatile unsigned long last_interrupt = 0;
volatile bool needToToggleDoor = false;

void handleButtonInterrupt()
{
  static volatile unsigned long lastButtonDown = 0;
  if (needToToggleDoor)
  {
    return;
  }
  if (millis() - lastButtonDown < 100)
  {
    //false alaram
    return;
  }
  needToToggleDoor = true;
  lastButtonDown = millis();
  // need_to_run_service = true;
}

void setupButton()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

void triggerToggleDoor()
{
  needToToggleDoor = false;
  if (!globalIsWifiConnected)
  {
    clickRfRemote();
    return;
  }
  bool b = (globalDoorState != DOOR_STATE_OPEN);
  globalDesiredDoorState = b ? DOOR_STATE_OPEN : DOOR_STATE_CLOSED;
  Serial.print("Button down. Door state is ");
  Serial.print(globalDoorState);
  Serial.print(" so toggling door to ");
  Serial.print(b);
  Serial.print(". New desired state is ");
  Serial.println(globalDesiredDoorState);
  changeCoverState(b);
  globalReadStateDelay = DELAY_BETWEEN_READ_STATE_FAST;
}

void loopButton()
{
  if (needToToggleDoor)
  {
    triggerToggleDoor();
    return;
  }
  if (globalDesiredDoorState == globalDoorState)
  {
    globalDesiredDoorState = DOOR_STATE_ANY;
    globalReadStateDelay = DELAY_BETWEEN_READ_STATE_NORMAL;
  }
}
