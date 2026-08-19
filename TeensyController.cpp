#include "TeensyController.h"

#include <Arduino.h>

#include "Config.h"

void initTeensyController() {
  Serial.begin(9600);
  Serial.println("Connected");

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
}

