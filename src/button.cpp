#include <Arduino.h>
#include "button.h"
#include "user_config.h"

volatile unsigned long last_interrupt = 0;
bool need_to_run_service = false;



void handleButtonInterrupt() {
  if (need_to_run_service) {
    return;
  }
  if (millis() - last_interrupt <100) {
    //false alaram
    return;
  }
  Serial.println("Button pushed");
  need_to_run_service = true;
  last_interrupt = millis();
}



void setupButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

void indicateError() {
  for (int i=0; i<20; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);
  }
}


void indicateRunningService() {
  for (int i=0; i<2; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void indicateWaitingForStateToChange() {
  digitalWrite(LED_PIN, LOW);
  delay(150);
  digitalWrite(LED_PIN, HIGH);
  delay(150);
}

void indicateWaitingForWiFi() {
  digitalWrite(LED_PIN,1);
  delay(10);
  digitalWrite(LED_PIN,0);
  delay(1000);
}
