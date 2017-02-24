#include <Arduino.h>

int num_seconds_to_sleep = 5;

// sample variable to show loss of SRAM after wakeup
int value = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("in setup()");

  Serial.print("value="); Serial.println(value);

  // Set GPIO16 (=D0) pin mode to allow for deep sleep
  // Connect D0 to RST for this to work. Unplug when flashing.
  pinMode(D0,WAKEUP_PULLUP);
}

void loop() {
  Serial.println("in loop()");
  value = 3;
  Serial.print("value="); Serial.println(value);

  // go to sleep, default mode (keep RF active)
  ESP.deepSleep(num_seconds_to_sleep * 1000000, RF_DEFAULT);
}
