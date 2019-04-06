#include <Arduino.h>
#include "wifi.h"
#include "button.h"
#include "led.h"
#include "ha.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n-----------------------------------------\nHello from Smart Garage button");
  setupWifi();
  setupHa();
  setupLed();
  setupButton();
}

// void changeDoorState(bool isOpen)
// {
//   if (isOpen != is_door_open)
//   {
//     Serial.printf("state changed from %d to %d\n", is_door_open, isOpen);
//   }
//   is_door_open = isOpen;
//   digitalWrite(LED_PIN, !is_door_open);
// }

void loop()
{
  loopWifi();
  loopButton();
  loopHa();
  loopLed();
  // if (need_to_run_service)
  // {
  //   indicateRunningService();
  //   waiting_for_state_change = true;
  //   need_to_run_service = false;
  //   Serial.println("Running servce");
  //   changeCoverState(!is_door_open);
  //   desired_state = !desired_state;
  // }
  //   bool newState = (temp == 1);
  // bool isNewStateSameAsDesired = (newState == desired_state);
  // changeDoorState(newState);

  // if (!waiting_for_state_change)
  // {
  //   return;
  // }
  // if (isNewStateSameAsDesired)
  // {
  //   changeDoorState(newState);
  //   waiting_for_state_change = false;
  //   return;
  // }
  // indicateWaitingForStateToChange();
}
